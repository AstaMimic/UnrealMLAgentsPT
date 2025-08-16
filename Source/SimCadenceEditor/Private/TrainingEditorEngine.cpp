#include "TrainingEditorEngine.h"

void UTrainingEditorEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);
}

void UTrainingEditorEngine::Tick(float DeltaSeconds, bool bIdleMode)
{
	Super::Tick(DeltaSeconds, bIdleMode);
}

void UTrainingEditorEngine::RedrawViewports(bool bShouldPresent)
{
        Super::RedrawViewports(bShouldPresent);
}
