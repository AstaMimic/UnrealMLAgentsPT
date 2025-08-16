// SimCadenceEngineSubsystem.h
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Engine/World.h"
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
    UFUNCTION()
    void OnWorldInit(UWorld* World, const UWorld::InitializationValues IVS);

    void ApplyFromSettings(UWorld* World);
    ASimCadencePhysicsBridge* GetOrSpawnPhysicsBridge(UWorld* World);

    FDelegateHandle WorldInitHandle;
};

