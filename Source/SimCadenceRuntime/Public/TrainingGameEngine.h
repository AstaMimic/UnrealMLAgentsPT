#pragma once
#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "TrainingGameEngine.generated.h"

UCLASS(config = Engine)
class SIMCADENCERUNTIME_API UTrainingGameEngine : public UGameEngine
{
	GENERATED_BODY()
protected:
	virtual void RedrawViewports(bool bShouldPresent) override;
};