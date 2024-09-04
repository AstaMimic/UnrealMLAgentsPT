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
    float RaysPerDirection;

    // The length of the ray
    UPROPERTY(EditAnywhere, Category = "Ray Sensor")
    float MaxRayDegrees;

    /// Starting height offset of ray from center of actor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ray Perception")
    float StartOffset;

    /// The axis relative to which rays will be cast (e.g., X, Y, Z)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ray Perception")
    ERayAxis RayAxis;

    /// Yaw offset to compensate for initial rotation of the mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ray Perception")
    float YawOffset;

private:
	FRayInput GetRayInput();
    TArray<float> GetRayAngles();
};