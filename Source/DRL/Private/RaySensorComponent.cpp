#include "RaySensorComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

TArray<TScriptInterface<IISensor>> URaySensorComponent::CreateSensors_Implementation() {
	URaySensor* RaySensor = NewObject<URaySensor>();
	FRayInput RayInput = GetRayInput();
	RaySensor->Initialize(SensorName, RayInput);
	return TArray<TScriptInterface<IISensor>> { RaySensor };
}

FRayInput URaySensorComponent::GetRayInput() {
	
	FRayInput RayInput;
	RayInput.Origin = GetOwner()->GetActorLocation();
	RayInput.IgnoredActor = GetOwner();
	RayInput.Rotation = GetOwner()->GetActorQuat();
	RayInput.NumRays = NumRays;
	RayInput.ForwardAxis = ForwardAxis;
	RayInput.RayLength = RayLength;
	return RayInput;
}
