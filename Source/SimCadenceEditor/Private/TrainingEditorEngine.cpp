#include "TrainingEditorEngine.h"
#include "SimCadenceEngineSubsystem.h"
#include "Engine/Engine.h"

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
	bool bPresent = bShouldPresent;
	if (USimCadenceEngineSubsystem* Sub = GEngine ? GEngine->GetEngineSubsystem<USimCadenceEngineSubsystem>() : nullptr)
	{
		bPresent = Sub->ShouldSubmitFrame();
	}
	Super::RedrawViewports(bPresent);
}
