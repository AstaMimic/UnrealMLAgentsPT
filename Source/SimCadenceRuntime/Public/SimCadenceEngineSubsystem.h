// SimCadenceEngineSubsystem.h
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Engine/World.h" // UWorld, FWorldInitializationValues, FWorldDelegates
#include "SimCadenceEngineSubsystem.generated.h"

class ASimCadencePhysicsBridge;

UCLASS()
class SIMCADENCERUNTIME_API USimCadenceEngineSubsystem : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    // Note: UHT cannot parse nested types in reflected signatures.
    // Use the top-level alias FWorldInitializationValues and do not mark
    // the function with UFUNCTION.
    void OnWorldInit(UWorld* World, const FWorldInitializationValues IVS);

    void ApplyFromSettings(UWorld* World);
    ASimCadencePhysicsBridge* GetOrSpawnPhysicsBridge(UWorld* World);

    FDelegateHandle WorldInitHandle;
};

