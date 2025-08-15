#pragma once
#include "CoreMinimal.h"
#include "Engine/EngineCustomTimeStep.h"
#include "SimFixedCustomTimeStep.generated.h"

UCLASS()
class SIMCADENCECONTROLLER_API USimFixedCustomTimeStep : public UEngineCustomTimeStep
{
	GENERATED_BODY()
public:
	USimFixedCustomTimeStep();

	virtual bool Initialize(class UEngine* InEngine) override;
	virtual void Shutdown(class UEngine* InEngine) override;
	virtual bool UpdateTimeStep(class UEngine* InEngine) override;

private:
	double FixedDeltaSeconds = 1.0 / 60.0;
};