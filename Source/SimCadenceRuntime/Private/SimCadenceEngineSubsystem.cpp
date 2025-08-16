#include "SimCadenceEngineSubsystem.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Misc/App.h"
#include "SimCadenceSettings.h"
#include "SimFixedCustomTimeStep.h"
#include "SimCadencePhysicsBridge.h"

void USimCadenceEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    WorldInitHandle = FWorldDelegates::OnPostWorldInitialization.AddUObject(
        this, &USimCadenceEngineSubsystem::OnWorldInit);
}

void USimCadenceEngineSubsystem::Deinitialize()
{
    FWorldDelegates::OnPostWorldInitialization.Remove(WorldInitHandle);
    Super::Deinitialize();
}

void USimCadenceEngineSubsystem::OnWorldInit(UWorld* World, const UWorld::InitializationValues IVS)
{
    if (!World)
    {
        return;
    }

    ApplyFromSettings(World);
    GetOrSpawnPhysicsBridge(World);
}

void USimCadenceEngineSubsystem::ApplyFromSettings(UWorld* World)
{
    const USimCadenceSettings* Settings = GetDefault<USimCadenceSettings>();
    if (!Settings)
    {
        return;
    }

    FApp::SetUseFixedTimeStep(Settings->bUseFixedTimestep);

    if (GEngine && Settings->bInstallCustomTimeStep)
    {
        if (!GEngine->GetCustomTimeStep() ||
            !GEngine->GetCustomTimeStep()->IsA(USimFixedCustomTimeStep::StaticClass()))
        {
            USimFixedCustomTimeStep* TimeStep = NewObject<USimFixedCustomTimeStep>(GetTransientPackage());
            GEngine->SetCustomTimeStep(TimeStep);
        }
    }
}

ASimCadencePhysicsBridge* USimCadenceEngineSubsystem::GetOrSpawnPhysicsBridge(UWorld* World)
{
    if (!World)
    {
        return nullptr;
    }

    FActorSpawnParameters SP;
    SP.ObjectFlags = RF_Transient;
    SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return World->SpawnActor<ASimCadencePhysicsBridge>(ASimCadencePhysicsBridge::StaticClass(), SP);
}

