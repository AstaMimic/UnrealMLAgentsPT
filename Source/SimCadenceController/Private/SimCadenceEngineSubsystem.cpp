#include "SimCadenceEngineSubsystem.h"
#include "SimCadenceSettings.h"
#include "SimFixedCustomTimeStep.h"
#include "Engine/Engine.h"
#include "Engine/GameEngine.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "SimCadencePhysicsBridge.h"

static void SetCVarIfPresent(const TCHAR* Name, int32 Value)
{
	if (IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(Name))
	{
		Var->Set(Value, ECVF_SetByCode);
	}
	else
	{
		UE_LOG(LogTemp, Verbose, TEXT("[SimCadence] CVar '%s' not found at init; skipping."), Name);
	}
}

void USimCadenceEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	ApplyFromSettings();
	const USimCadenceSettings* S = USimCadenceSettings::Get();

	WorldInitHandle =
		FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &USimCadenceEngineSubsystem::OnWorldInit);
	WorldCleanupHandle =
		FWorldDelegates::OnWorldCleanup.AddUObject(this, &USimCadenceEngineSubsystem::OnWorldDestroyed);

	switch (S->Mode)
	{
		case ESimCadenceMode::Realtime:
			ApplyRealtimeMode();
			break;
		case ESimCadenceMode::TrainingRendered:
			ApplyTrainingMode(false);
			break;
		case ESimCadenceMode::TrainingHeadless:
			ApplyTrainingMode(true);
			break;
	}
}

void USimCadenceEngineSubsystem::ApplyFromSettings()
{
	const USimCadenceSettings* S = USimCadenceSettings::Get();
	switch (S->Mode)
	{
		case ESimCadenceMode::Realtime:
			ApplyRealtimeMode();
			break;
		case ESimCadenceMode::TrainingRendered:
			ApplyTrainingMode(false);
			break;
		case ESimCadenceMode::TrainingHeadless:
			ApplyTrainingMode(true);
			break;
	}
}

void USimCadenceEngineSubsystem::ReapplyFromSettings()
{
	ApplyFromSettings();
}

void USimCadenceEngineSubsystem::Deinitialize()
{
	if (WorldInitHandle.IsValid())
	{
		FWorldDelegates::OnPostWorldInitialization.Remove(WorldInitHandle);
	}
	if (WorldCleanupHandle.IsValid())
	{
		FWorldDelegates::OnWorldCleanup.Remove(WorldCleanupHandle);
	}
	RemoveCustomTimeStep();
}

bool USimCadenceEngineSubsystem::ShouldSubmitFrame()
{
	const USimCadenceSettings* S = USimCadenceSettings::Get();
	const double			   Now = FPlatformTime::Seconds();

	if (S->Mode == ESimCadenceMode::TrainingHeadless)
	{
		return false;
	}

	if (PresentInterval <= 0.0)
	{
		return true;
	}

	if (Now - LastPresentedTime >= PresentInterval)
	{
		LastPresentedTime = Now;
		return true;
	}
	return false;
}

void USimCadenceEngineSubsystem::ApplyRealtimeMode()
{
	const USimCadenceSettings* S = USimCadenceSettings::Get();

	RemoveCustomTimeStep();

	if (S->bUncapRealtimeRendering)
	{
		SetCVarIfPresent(TEXT("r.VSync"), 0);
		SetCVarIfPresent(TEXT("t.MaxFPS"), 0);
	}

	ConfigurePhysicsSubstepping();

	PresentInterval = 0.0;
}

void USimCadenceEngineSubsystem::ApplyTrainingMode(bool bHeadless)
{
	const USimCadenceSettings* S = USimCadenceSettings::Get();

	InstallCustomTimeStep();

	if (bHeadless)
	{
		PresentInterval = -1.0;
	}
	else
	{
		if (S->bUncapInTraining)
		{
			PresentInterval = 0.0;
		}
		else
		{
			PresentInterval = 1.0 / FMath::Max(1.f, S->TrainingRenderCapHz);
		}
	}

	SetCVarIfPresent(TEXT("r.VSync"), 0);
	SetCVarIfPresent(TEXT("t.MaxFPS"), 0);

	if (S->bDisableAudioInTraining)
	{
		SetCVarIfPresent(TEXT("au.RenderAudio"), 0);
	}
}

void USimCadenceEngineSubsystem::InstallCustomTimeStep()
{
	if (GEngine && !CustomTS.IsValid())
	{
		USimFixedCustomTimeStep* NewTS = NewObject<USimFixedCustomTimeStep>(GEngine);
		GEngine->SetCustomTimeStep(NewTS);
		CustomTS = NewTS;
	}
}

void USimCadenceEngineSubsystem::RemoveCustomTimeStep()
{
	if (GEngine)
	{
		GEngine->SetCustomTimeStep(nullptr);
	}
	CustomTS.Reset();
	FApp::SetUseFixedTimeStep(false);
}

void USimCadenceEngineSubsystem::ConfigurePhysicsSubstepping()
{
	const USimCadenceSettings* S = USimCadenceSettings::Get();
	if (UPhysicsSettings* PS = UPhysicsSettings::Get())
	{
		if (S->bEnablePhysicsSubstepping)
		{
			PS->bSubstepping = true;
			PS->MaxSubstepDeltaTime = 1.0f / FMath::Max(1.f, S->FixedHz);
			PS->MaxSubsteps = 8;
		}
	}
}

void USimCadenceEngineSubsystem::OnWorldInit(UWorld* World, const UWorld::InitializationValues IVS)
{
	if (!World || World->IsPreviewWorld())
		return;
	ApplyFromSettings();
	GetOrSpawnPhysicsBridge(World);
}

void USimCadenceEngineSubsystem::OnWorldDestroyed(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	Bridges.Remove(World);
}

ASimCadencePhysicsBridge* USimCadenceEngineSubsystem::GetOrSpawnPhysicsBridge(UWorld* World)
{
	if (!World)
		return nullptr;
	if (TWeakObjectPtr<ASimCadencePhysicsBridge>* Found = Bridges.Find(World))
	{
		return Found->Get();
	}

	FActorSpawnParameters SP;
	SP.ObjectFlags = RF_Transient;
	SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ASimCadencePhysicsBridge* Bridge =
		World->SpawnActor<ASimCadencePhysicsBridge>(ASimCadencePhysicsBridge::StaticClass());
	Bridges.Add(World, Bridge);
	return Bridge;
}