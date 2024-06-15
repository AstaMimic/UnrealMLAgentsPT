// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/NoExportTypes.h"
#include "Delegates/DelegateCombinations.h"
#include "CommunicatorInterface.generated.h"

// Structs

USTRUCT(BlueprintType)
struct FCommunicatorInitParameters
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 Port;

    UPROPERTY(BlueprintReadWrite)
    FString Name;

    UPROPERTY(BlueprintReadWrite)
    FString UnrealPackageVersion;

    UPROPERTY(BlueprintReadWrite)
    FString UnrealCommunicationVersion;

    UPROPERTY(BlueprintReadWrite)
    FString CSharpCapabilities;
};

USTRUCT(BlueprintType)
struct FUnrealRLInitParameters
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 Seed;

    UPROPERTY(BlueprintReadWrite)
    int32 NumAreas;

    UPROPERTY(BlueprintReadWrite)
    FString PythonLibraryVersion;

    UPROPERTY(BlueprintReadWrite)
    FString PythonCommunicationVersion;

    UPROPERTY(BlueprintReadWrite)
    FString TrainerCapabilities;
};

USTRUCT(BlueprintType)
struct FUnrealRLInputParameters
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    bool bIsTraining;
};


// Delegates

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuitCommandHandler);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResetCommandHandler);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRLInputReceivedHandler, const FUnrealRLInputParameters&, InputParams);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommunicatorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMUNICATOR_API ICommunicatorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
    // Events
    virtual FQuitCommandHandler& OnQuitCommandReceived() = 0;
    virtual FResetCommandHandler& OnResetCommandReceived() = 0;
    virtual FRLInputReceivedHandler& OnRLInputReceived() = 0; // Add this line

    // Methods
    virtual bool Initialize(const FCommunicatorInitParameters& InitParameters, FUnrealRLInitParameters& InitParametersOut) = 0;
    virtual void SubscribeBrain(const FString& Name, const FString& ActionSpec) = 0;
    virtual void PutObservations(const FString& BrainKey, const FString& AgentInfo, const TArray<FString>& Sensors) = 0;
    virtual void DecideBatch() = 0;
    virtual FString GetActions(const FString& Key, int32 AgentId) = 0;
};

