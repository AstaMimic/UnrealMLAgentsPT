// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BehaviorParameters.h"
#include "ISensor.h"
#include "AgentInfo.h"
#include "VectorSensor.h"
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


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRL_API UAgent : public UActorComponent
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

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "YourCategory")
    void Heuristic(const FActionBuffers& ActionsOut);
    virtual void Heuristic_Implementation(const FActionBuffers& ActionsOut);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "YourCategory")
    void CollectObservations(UVectorSensor* Sensor);
    virtual void CollectObservations_Implementation(UVectorSensor* Sensor);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "YourCategory")
    void Initialize();
    virtual void Initialize_Implementation();
   
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "YourCategory")
    void OnActionReceived(FActionBuffers Actions);
    virtual void OnActionReceived_Implementation(FActionBuffers Actions);

    // TODO: Check if we need to implement WriteDiscreteActionMask. Link issue on build

    int32 MaxStep;


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

    UPROPERTY()
    UVectorSensor* CollectObservationsSensor;
};
