#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SimCadenceSettings.generated.h"

UENUM(BlueprintType)
enum class ESimCadenceMode : uint8
{
	Realtime		 UMETA(DisplayName = "Realtime"),
	TrainingRendered UMETA(DisplayName = "Training Rendered"),
	TrainingHeadless UMETA(DisplayName = "Training Headless")
};

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Sim Cadence"))
class SIMCADENCECONTROLLER_API USimCadenceSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Config, Category = "General", meta = (ClampMin = "1.0"))
	float FixedHz = 60.f;

	UPROPERTY(EditAnywhere, Config, Category = "General")
	ESimCadenceMode Mode = ESimCadenceMode::Realtime;

	// Realtime
	UPROPERTY(EditAnywhere, Config, Category = "Realtime")
	bool bUncapRealtimeRendering = true;

	UPROPERTY(EditAnywhere, Config, Category = "Realtime",
		meta = (ToolTip = "Enable Chaos substepping and set MaxSubstepDeltaTime = 1/FixedHz"))
	bool bEnablePhysicsSubstepping = true;

	// Training
	UPROPERTY(EditAnywhere, Config, Category = "Training")
	bool bUncapInTraining = true;

	UPROPERTY(EditAnywhere, Config, Category = "Training", meta = (ClampMin = "1.0"))
	float TrainingRenderCapHz = 60.f;

	UPROPERTY(EditAnywhere, Config, Category = "Training")
	bool bDisableAudioInTraining = true;

	UPROPERTY(EditAnywhere, Config, Category = "Logging")
	bool bVerboseLogs = false;

	static const USimCadenceSettings* Get() { return GetDefault<USimCadenceSettings>(); }

#if WITH_EDITOR
public:
	virtual FName GetContainerName() const override { return TEXT("Project"); }	 // Project Settings
	virtual FName GetCategoryName() const override { return TEXT("Plugins"); }	 // Category in sidebar
	virtual FName GetSectionName() const override { return TEXT("SimCadence"); } // Entry label
	virtual FText GetSectionText() const override { return FText::FromString(TEXT("Sim Cadence")); }
	virtual FText GetSectionDescription() const override
	{
		return FText::FromString(TEXT("Physics-authoritative cadence and training controls."));
	}
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};