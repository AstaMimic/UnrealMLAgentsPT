#include "SimCadenceSettings.h"
#include "Engine/Engine.h"

#if WITH_EDITOR
void USimCadenceSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
        if (GEngine)
        {
                // Settings changes will take effect on the next initialization cycle.
        }
}
#endif

