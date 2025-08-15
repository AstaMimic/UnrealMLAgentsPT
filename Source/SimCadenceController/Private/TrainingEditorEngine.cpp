#if WITH_SIMCADENCE_TRAINING_ENGINE
	#include "TrainingEditorEngine.h"
	#include "SimCadenceEngineSubsystem.h"
	#include "Engine/Engine.h"

void UTrainingEditorEngine::RedrawViewports(bool bShouldPresent)
{
	bool bPresent = bShouldPresent;
	if (USimCadenceEngineSubsystem* Sub = GEngine ? GEngine->GetEngineSubsystem<USimCadenceEngineSubsystem>() : nullptr)
	{
		bPresent = Sub->ShouldSubmitFrame();
	}
	Super::RedrawViewports(bPresent);
}
#endif // WITH_SIMCADENCE_TRAINING_ENGINE
