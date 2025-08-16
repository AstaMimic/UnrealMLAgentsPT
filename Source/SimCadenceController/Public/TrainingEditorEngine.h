#pragma once

#include "CoreMinimal.h"

#if WITH_SIMCADENCE_TRAINING_ENGINE
	#include "Editor/EditorEngine.h"
	#include "TrainingEditorEngine.generated.h"

UCLASS(config = Engine)
class SIMCADENCECONTROLLER_API UTrainingEditorEngine : public UEditorEngine
{
	GENERATED_BODY()

protected:
	virtual void RedrawViewports(bool bShouldPresent) override;
};
#else
	#include "Engine/Engine.h"
	#include "TrainingEditorEngine.generated.h"

UCLASS(config = Engine)
class SIMCADENCECONTROLLER_API UTrainingEditorEngine : public UEngine
{
	GENERATED_BODY()
};
#endif
