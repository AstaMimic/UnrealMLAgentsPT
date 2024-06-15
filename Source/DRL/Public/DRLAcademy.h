// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpcCommunicator.h"
#include "DRLAcademy.generated.h"

/**
 * 
 */
UCLASS()
class DRL_API UDRLAcademy : public UObject
{
	GENERATED_BODY()

public:
	bool bIsInitialized = false;

	static UDRLAcademy* GetInstance();
	
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

	// Environment variables
	bool bInitialized;
	int32 InferenceSeed;
	int32 NumAreas;

    UPROPERTY()
    URpcCommunicator* RpcCommunicator;

	void Initialize();
	void InitializeEnvironment();
	void ParseCommandLineArgs();

	// Bind to events handler
	UFUNCTION()
	void Dispose();

	UFUNCTION()
	void OnQuitCommandReceived();
	
	UFUNCTION()
    void OnResetCommand();
};
