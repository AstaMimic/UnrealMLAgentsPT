#pragma once

#include "CoreMinimal.h"
#include "Editor/UnrealEdEngine.h"
#include "TrainingEditorEngine.generated.h"

class IEngineLoop;

UCLASS(Config = Engine, Transient)
class SIMCADENCEEDITOR_API UTrainingEditorEngine : public UUnrealEdEngine
{
	GENERATED_BODY()

public:
	virtual void Init(IEngineLoop* InEngineLoop) override;
	virtual void Tick(float DeltaSeconds, bool bIdleMode) override;

protected:
	virtual void RedrawViewports(bool bShouldPresent) override;
};
