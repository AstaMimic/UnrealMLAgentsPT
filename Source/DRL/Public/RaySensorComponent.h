#pragma once

#include "CoreMinimal.h"
#include "RaySensor.h"
#include "SensorComponent.h"
#include "RaySensorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRL_API URaySensorComponent : public USensorComponent
{
	GENERATED_BODY()

public:

	virtual TArray<TScriptInterface<IISensor>> CreateSensors_Implementation() override;

    UPROPERTY(EditAnywhere, Category="Ray Sensor")
	FString SensorName = "RaySensor";
	
    // The length of the ray
    UPROPERTY(EditAnywhere, Category = "Ray Sensor")
    float RayLength;

    // The length of the ray
    UPROPERTY(EditAnywhere, Category = "Ray Sensor")
    int32 NumRays;

    // The axis
    UPROPERTY(EditAnywhere, Category="Ray Sensor")
    EForwardAxis ForwardAxis;


private:
	FRayInput GetRayInput();
};