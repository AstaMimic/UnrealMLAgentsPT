#include "SimCadencePhysicsBridge.h"
#include "SimCadenceSettings.h"
#include "Async/Async.h"

ASimCadencePhysicsBridge::ASimCadencePhysicsBridge()
{
	PrimaryActorTick.bCanEverTick = false;
	bAsyncPhysicsTickEnabled = true;
	const USimCadenceSettings* S = USimCadenceSettings::Get();
	FixedDeltaSeconds = 1.0 / FMath::Max(1.f, S->FixedHz);
}

void ASimCadencePhysicsBridge::BeginPlay()
{
	Super::BeginPlay();
	const USimCadenceSettings* S = USimCadenceSettings::Get();
	FixedDeltaSeconds = 1.0 / FMath::Max(1.f, S->FixedHz);
}

void ASimCadencePhysicsBridge::AsyncPhysicsTickActor(float DeltaTime, float SimTime)
{
	Accumulator += DeltaTime;
	while (Accumulator + KINDA_SMALL_NUMBER >= FixedDeltaSeconds)
	{
		Accumulator -= FixedDeltaSeconds;
		const float Step = (float)FixedDeltaSeconds;
		AsyncTask(ENamedThreads::GameThread, [this, Step]() {
			if (IsValid(this))
			{
				OnFixedStep.Broadcast(Step);
			}
		});
	}
}