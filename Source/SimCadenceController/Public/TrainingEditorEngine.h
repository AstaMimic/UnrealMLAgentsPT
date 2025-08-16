#pragma once

#include "CoreMinimal.h"

#if WITH_SIMCADENCE_TRAINING_ENGINE
#include "Editor/EditorEngine.h"
#define UTrainingEditorEngineBase UEditorEngine
#else
#include "Engine/Engine.h"
#define UTrainingEditorEngineBase UEngine
#endif

#include "TrainingEditorEngine.generated.h"

UCLASS(config = Engine)
class SIMCADENCECONTROLLER_API UTrainingEditorEngine : public UTrainingEditorEngineBase
{
    GENERATED_BODY()

protected:
#if WITH_SIMCADENCE_TRAINING_ENGINE
    virtual void RedrawViewports(bool bShouldPresent) override;
#endif
};

#undef UTrainingEditorEngineBase

