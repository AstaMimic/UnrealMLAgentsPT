// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPolicy.h"
#include "ActionSpec.h"
#include "HeuristicPolicy.h"
#include "ActuatorManager.h"
#include "BrainParameters.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "BehaviorParameters.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPolicyUpdated, bool, IsInHeuristicMode);

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
    HeuristicOnly UMETA(DisplayName = "Heuristic Only"),
    InferenceOnly UMETA(DisplayName = "Inference Only"),
    Default UMETA(DisplayName = "Default")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRL_API UBehaviorParameters : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBehaviorParameters();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MLAgents")
    int32 MaxStep;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MLAgents")
    FString BehaviorName = "My Behavior";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MLAgents")
    int32 TeamId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MLAgents")
    bool bUseChildSensors = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MLAgents")
    bool bUseChildActuators = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MLAgents")
    bool bDeterministicInference = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MLAgents")
    EBehaviorType BehaviorType;

    UPROPERTY(BlueprintAssignable, Category="MLAgents")
    FPolicyUpdated OnPolicyUpdated;

    /** The associated BrainParameters for this behavior. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior Parameters")
    FBrainParameters BrainParameters;

    UFUNCTION()
    bool IsInHeuristicMode();

private:
    void UpdateAgentPolicy();
    UHeuristicPolicy* InitializeHeuristicPolicy(const FActionSpec& ActionSpec, UActuatorManager* ActuatorManager);
    TScriptInterface<IIPolicy> GeneratePolicy(const FActionSpec& ActionSpec, UActuatorManager* ActuatorManager);
    FString GetFullyQualifiedBehaviorName();

    // Grant Access to UAgent
    friend class UAgent;
};
