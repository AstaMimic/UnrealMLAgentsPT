#pragma once

#include "CoreMinimal.h"

#if WITH_SIMCADENCE_TRAINING_ENGINE
        #include "UnrealEd/UnrealEdEngine.h"
        #if (ENGINE_MAJOR_VERSION >= 5)
                #define TRAINING_EDITOR_ENGINE_SUPER UUnrealEdEngine
        #else
                #define TRAINING_EDITOR_ENGINE_SUPER UEditorEngine
        #endif
#else
        #include "Engine/Engine.h"
        #define TRAINING_EDITOR_ENGINE_SUPER UEngine
#endif

#include "TrainingEditorEngine.generated.h"

UCLASS(config = Engine)
class SIMCADENCECONTROLLER_API UTrainingEditorEngine : public TRAINING_EDITOR_ENGINE_SUPER
{
	GENERATED_BODY()

#if WITH_SIMCADENCE_TRAINING_ENGINE
protected:
	virtual void RedrawViewports(bool bShouldPresent) override;
#endif
};
