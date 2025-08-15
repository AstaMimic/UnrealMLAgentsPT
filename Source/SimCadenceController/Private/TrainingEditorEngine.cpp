#if WITH_EDITOR && UE_EDITOR
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
#endif // WITH_EDITOR && UE_EDITOR
