#pragma once

#if WITH_SIMCADENCE_TRAINING_ENGINE
	#include "CoreMinimal.h"
	#include "Editor/EditorEngine.h"
	#include "TrainingEditorEngine.generated.h"

UCLASS(config = Engine)
class SIMCADENCECONTROLLER_API UTrainingEditorEngine : public UEditorEngine
{
	GENERATED_BODY()

protected:
	virtual void RedrawViewports(bool bShouldPresent) override;
};

#endif // WITH_SIMCADENCE_TRAINING_ENGINE
