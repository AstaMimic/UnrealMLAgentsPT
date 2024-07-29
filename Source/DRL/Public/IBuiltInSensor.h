#pragma once

#include "CoreMinimal.h"
#include "IBuiltInSensor.generated.h"

UENUM(BlueprintType)
enum class EBuiltInSensorType : uint8
{
    // Default Sensor type if it cannot be determined.
    Unknown UMETA(DisplayName = "Unknown"),

    // The Vector sensor used by the agent.
    VectorSensor UMETA(DisplayName = "Vector Sensor"),

    // The Stacking Sensor type.
    StackingSensor UMETA(DisplayName = "Stacking Sensor"),

    // The Ray Sensor
    RaySensor UMETA(DisplayName = "Ray Sensor")
};



UINTERFACE(MinimalAPI)
class UBuiltInSensor : public UInterface
{
    GENERATED_BODY()
};

class DRL_API IBuiltInSensor
{
    GENERATED_BODY()

public:
    // Return the corresponding BuiltInSensorType for the sensor.
    virtual EBuiltInSensorType GetBuiltInSensorType() const = 0;
};