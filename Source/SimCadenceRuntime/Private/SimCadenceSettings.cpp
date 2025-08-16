#include "SimCadenceSettings.h"
#include "SimCadenceEngineSubsystem.h"
#include "Engine/Engine.h"

#if WITH_EDITOR
void USimCadenceSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (GEngine)
	{
		if (USimCadenceEngineSubsystem* Sub = GEngine->GetEngineSubsystem<USimCadenceEngineSubsystem>())
		{
			Sub->ReapplyFromSettings();
		}
	}
}
#endif

