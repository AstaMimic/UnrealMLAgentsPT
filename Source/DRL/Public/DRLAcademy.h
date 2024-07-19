// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "UObject/NoExportTypes.h"
#include "RpcCommunicator.h"
#include "RecursionChecker.h"
#include "RemotePolicy.h"
#include "Tickable.h"
#include "DRLAcademy.generated.h"

// Define the delegate types
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDecideActionDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestroyActionDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAgentIncrementStepDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAgentPreStepDelegate, int32, StepCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAgentSendStateDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAgentActDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAgentForceResetDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnvironmentReset);

/**
 * 
 */
UCLASS()
class DRL_API UDRLAcademy : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	bool bIsInitialized = false;
	int32 TotalStepCount;
	int32 EpisodeCount;
	int32 StepCount;
	int32 NumAreas;
	int32 InferenceSeed;

	static UDRLAcademy* GetInstance();

	void Dispose();
	void EnvironmentStep();
	bool IsCommunicatorOn();

    // Declare events
    UPROPERTY(BlueprintAssignable, Category = "Academy Events")
    FDecideActionDelegate OnDecideAction;

    UPROPERTY(BlueprintAssignable, Category = "Academy Events")
    FDestroyActionDelegate OnDestroyAction;

    UPROPERTY(BlueprintAssignable, Category = "Academy Events")
    FAgentIncrementStepDelegate OnAgentIncrementStep;

    UPROPERTY(BlueprintAssignable, Category = "Academy Events")
    FAgentPreStepDelegate OnAgentPreStep;

    UPROPERTY(BlueprintAssignable, Category = "Academy Events")
    FAgentSendStateDelegate OnAgentSendState;

    UPROPERTY(BlueprintAssignable, Category = "Academy Events")
    FAgentActDelegate OnAgentAct;

    UPROPERTY(BlueprintAssignable, Category = "Academy Events")
    FAgentForceResetDelegate OnAgentForceReset;

    UPROPERTY(BlueprintAssignable, Category = "Academy Events")
    FOnEnvironmentReset OnEnvironmentReset;

	
private:
	UDRLAcademy();

	// Versions
	const FString ApiVersion = "0.1.0";
	const FString PackageVersion = "1.0.0.dev0";
	
	// Command line port
	int32 Port = 5004;
	FString PortCommandLineFlag = "mlAgentPort=";

	// Store Academy instance
	static UDRLAcademy* Instance;

	// Detect an Academy step called by user code that is also called by the Academy
	URecursionChecker* StepRecursionChecker = NewObject<URecursionChecker>();

	// Environment variables
	bool bInitialized;
	bool bEnableStepping;
	bool bHadFirstReset;


    UPROPERTY()
    URpcCommunicator* RpcCommunicator;

	void LazyInitialize();
	void InitializeEnvironment();
	void ParseCommandLineArgs();
	void ForcedFullReset();
	void EnvironmentReset();
	void ResetActions();

	UFUNCTION()
    void OnResetCommand();

	UFUNCTION()
	void OnQuitCommandReceived();

	friend class URemotePolicy;

};
