#pragma once

#include "CoreMinimal.h"

#if WITH_SIMCADENCE_TRAINING_ENGINE
	#include "Editor/EditorEngine.h"
using FTrainingEditorEngineSuper = UEditorEngine;
#else
	#include "Engine/Engine.h"
using FTrainingEditorEngineSuper = UEngine;
#endif

#include "TrainingEditorEngine.generated.h"

UCLASS(config = Engine)
class SIMCADENCECONTROLLER_API UTrainingEditorEngine : public FTrainingEditorEngineSuper
{
	GENERATED_BODY()

#if WITH_SIMCADENCE_TRAINING_ENGINE
protected:
	virtual void RedrawViewports(bool bShouldPresent) override;
#endif
};
