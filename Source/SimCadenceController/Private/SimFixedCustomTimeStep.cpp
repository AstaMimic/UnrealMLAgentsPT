#include "SimFixedCustomTimeStep.h"
#include "SimCadenceSettings.h"
#include "Misc/App.h"

USimFixedCustomTimeStep::USimFixedCustomTimeStep()
{
	const USimCadenceSettings* S = USimCadenceSettings::Get();
	FixedDeltaSeconds = 1.0 / FMath::Max(1.f, S->FixedHz);
}

bool USimFixedCustomTimeStep::Initialize(UEngine* InEngine)
{
	FApp::SetUseFixedTimeStep(true);
	FApp::SetFixedDeltaTime(FixedDeltaSeconds);
	return true;
}

void USimFixedCustomTimeStep::Shutdown(UEngine* InEngine)
{
	FApp::SetUseFixedTimeStep(false);
}

bool USimFixedCustomTimeStep::UpdateTimeStep(UEngine* InEngine)
{
	const USimCadenceSettings* S = USimCadenceSettings::Get();
	const double			   Target = 1.0 / FMath::Max(1.f, S->FixedHz);
	if (Target != FixedDeltaSeconds)
	{
		FixedDeltaSeconds = Target;
		FApp::SetFixedDeltaTime(FixedDeltaSeconds);
	}
	// Training never sleeps; engine runs as fast as possible.
	return true;
}