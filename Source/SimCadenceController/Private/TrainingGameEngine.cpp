#include "TrainingGameEngine.h"
#include "SimCadenceEngineSubsystem.h"
#include "Engine/Engine.h"

void UTrainingGameEngine::RedrawViewports(bool bShouldPresent)
{
	bool bPresent = bShouldPresent;
	if (USimCadenceEngineSubsystem* Sub = GEngine ? GEngine->GetEngineSubsystem<USimCadenceEngineSubsystem>() : nullptr)
	{
		bPresent = Sub->ShouldSubmitFrame();
	}
	Super::RedrawViewports(bPresent);
}