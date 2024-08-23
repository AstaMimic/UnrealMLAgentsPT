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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Reward")
    void SetReward(float NewReward);

    UFUNCTION(BlueprintCallable, Category = "Reward")
    void AddReward(float NewReward);

    UFUNCTION(BlueprintCallable, Category = "Episode")
    void EndEpisode();

    UFUNCTION(BlueprintCallable, Category = "Episode")
    void EpisodeInterrupted();

    UFUNCTION(BlueprintCallable, Category = "AgentAction")
    void RequestDecision();

    UFUNCTION(BlueprintCallable, Category = "AgentAction")
    void RequestAction();

    UFUNCTION(BlueprintCallable, Category = "AgentAction")
    FActionBuffers GetStoredActionBuffers();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AgentObservation")
    void CollectObservations(UVectorSensor* Sensor);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AgentInit")
    void Initialize();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AgentAction")
    void OnActionReceived(const FActionBuffers& Actions) override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AgentAction")
    void WriteDiscreteActionMask(const TScriptInterface<IDiscreteActionMask>& ActionMask) override;

    // Read an FActionBuffers and return Discrete actions
    UFUNCTION(BlueprintCallable, Category = "AgentObservation")
    int32 GetDiscreteActions(const FActionBuffers& Actions, int32 Index);

    virtual void Heuristic(FActionBuffers& ActionsOut) override;

    UPROPERTY()
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
    UAgentVectorActuator* VectorActuator;

    UPROPERTY()
    UVectorSensor* CollectObservationsSensor;

    UPROPERTY()
    UActuatorManager* ActuatorManager;

    UPROPERTY()
    TArray<TScriptInterface<IISensor>> Sensors;

    UPROPERTY()
    FAgentInfo Info;

    UPROPERTY()
	TScriptInterface<IIPolicy> Brain;

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
