#pragma once

#if WITH_EDITOR && UE_EDITOR
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
#endif // WITH_EDITOR && UE_EDITOR
