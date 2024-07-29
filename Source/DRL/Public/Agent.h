// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BehaviorParameters.h"
#include "ISensor.h"
#include "AgentInfo.h"
#include "VectorSensor.h"
#include "VectorActuator.h"
#include "ActuatorManager.h"
#include "Agent.generated.h"

UENUM(BlueprintType)
enum class EDoneReason : uint8
{
    /** The episode was ended manually by calling EndEpisode. */
    DoneCalled UMETA(DisplayName = "Done Called"),

    /** The max steps for the Agent were reached. */
    MaxStepReached UMETA(DisplayName = "Max Step Reached"),

    /** The Agent was disabled. */
    Disabled UMETA(DisplayName = "Disabled")
};


/**
 * An implementation of VectorActuator specific to Agents.
 */
UCLASS()
class DRL_API UAgentVectorActuator : public UVectorActuator
{
    GENERATED_BODY()

public:

    virtual EBuiltInActuatorType GetBuiltInActuatorType() const override;
};


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRL_API UAgent : public UActorComponent, public IActionReceiver, public IHeuristicProvider
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAgent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    void SetReward(float NewReward);
    void AddReward(float NewReward);
    void EndEpisode();
    void EpisodeInterrupted();
    void RequestDecision();
    void RequestAction();
    FActionBuffers GetStoredActionBuffers();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AgentAction")
    void Heuristic(const FActionBuffers& ActionsOut) const override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AgentObservation")
    void CollectObservations(UVectorSensor* Sensor);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AgentInit")
    void Initialize();
   
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AgentAction")
    void OnActionReceived(const FActionBuffers& Actions) override;

    // TODO: Check if we need to implement WriteDiscreteActionMask. Link issue on build
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AgentAction")
    void WriteDiscreteActionMask(const TScriptInterface<IDiscreteActionMask>& ActionMask) override;

    int32 MaxStep;

    // Read an FActionBuffers and return Discrete actions
    UFUNCTION(BlueprintCallable, Category = "AgentObservation")
    int32 GetDiscreteActions(const FActionBuffers& Actions, int32 Index);


private:
    friend class UBehaviorParameters;

    void LazyInitialize();
    void OnEpisodeBegin();
    void InitializeActuators();
    void InitializeSensors();
    void ResetData();
    void SetGroupReward(float NewReward);
    void AddGroupReward(float Increment);
    void EndEpisodeAndReset(EDoneReason Reason);
    void NotifyAgentDone(EDoneReason DoneReason);
    void ReloadPolicy();
    void CleanupSensors();
    void UpdateSensors();
    void ResetSensors();
    void SendInfoToBrain();

    template<class T>
    void GetAllChildComponents(TArray<T*>& OutChildComponents);

    UFUNCTION()
    void AgentIncrementStep();

    UFUNCTION()
    void SendInfo();

    UFUNCTION()
    void DecideAction();
    
    UFUNCTION()
    void AgentStep();

    UFUNCTION()
    void AgentReset();

    UPROPERTY()
    UBehaviorParameters* PolicyFactory;

	UPROPERTY()
    UAgentVectorActuator* VectorActuator;

    UPROPERTY()
    UVectorSensor* CollectObservationsSensor;

    UPROPERTY()
    UActuatorManager* ActuatorManager;

    UPROPERTY()
    TArray<TScriptInterface<IISensor>> Sensors;

    FAgentInfo Info;
	IIPolicy* Brain;

    int32 GroupId;
    int32 StepCount;
    int32 CompletedEpisodes;
    int32 EpisodeId;

    float Reward;
    float GroupReward;
    float CumulativeReward;

    bool bInitialized;
    bool bRequestAction;
    bool bRequestDecision;

};
