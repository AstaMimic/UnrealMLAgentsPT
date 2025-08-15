#pragma once
#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "SimCadenceEngineSubsystem.generated.h"

class USimFixedCustomTimeStep;
class ASimCadencePhysicsBridge;

UCLASS()
class SIMCADENCECONTROLLER_API USimCadenceEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UFUNCTION(BlueprintCallable, Category = "SimCadence")
	void		 ReapplyFromSettings();
	virtual void Deinitialize() override;

	bool ShouldSubmitFrame();

	UFUNCTION(BlueprintCallable, Category = "SimCadence")
	ASimCadencePhysicsBridge* GetOrSpawnPhysicsBridge(UWorld* World);

private:
	void ApplyRealtimeMode();
	void ApplyFromSettings();
	void ApplyTrainingMode(bool bHeadless);
	void InstallCustomTimeStep();
	void RemoveCustomTimeStep();
	void ConfigurePhysicsSubstepping();
	void OnWorldInit(UWorld* World, const UWorld::InitializationValues IVS);
	void OnWorldDestroyed(UWorld* World, bool bSessionEnded, bool bCleanupResources);

private:
	TWeakObjectPtr<USimFixedCustomTimeStep> CustomTS;
	double									LastPresentedTime = 0.0;
	double									PresentInterval = 0.0;

	FDelegateHandle WorldInitHandle;
	FDelegateHandle WorldCleanupHandle;

	TMap<TWeakObjectPtr<UWorld>, TWeakObjectPtr<ASimCadencePhysicsBridge>> Bridges;
};