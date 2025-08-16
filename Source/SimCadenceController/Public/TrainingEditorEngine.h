#pragma once

#include "CoreMinimal.h"

#if WITH_SIMCADENCE_TRAINING_ENGINE
	#include "Editor/EditorEngine.h"
#else
	#include "Engine/Engine.h"
#endif

#include "TrainingEditorEngine.generated.h"

UCLASS(config = Engine)
class SIMCADENCECONTROLLER_API UTrainingEditorEngine
#if WITH_SIMCADENCE_TRAINING_ENGINE
	: public UEditorEngine
#else
	: public UEngine
#endif
{
	GENERATED_BODY()

#if WITH_SIMCADENCE_TRAINING_ENGINE
protected:
	virtual void RedrawViewports(bool bShouldPresent) override;
#endif
};
