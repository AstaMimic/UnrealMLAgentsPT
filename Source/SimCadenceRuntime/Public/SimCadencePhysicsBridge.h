#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimCadencePhysicsBridge.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFixedStepEvent, float, FixedDeltaTime);

UCLASS(NotBlueprintable, Transient)
class SIMCADENCERUNTIME_API ASimCadencePhysicsBridge : public AActor
{
	GENERATED_BODY()
public:
	ASimCadencePhysicsBridge();

	virtual bool ShouldTickIfViewportsOnly() const override { return true; }
	virtual void BeginPlay() override;
	virtual void AsyncPhysicsTickActor(float DeltaTime, float SimTime) override;

	UPROPERTY(BlueprintAssignable, Category = "SimCadence")
	FFixedStepEvent OnFixedStep;

private:
	double Accumulator = 0.0;
	double FixedDeltaSeconds = 1.0 / 60.0;
};