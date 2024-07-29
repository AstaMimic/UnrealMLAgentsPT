#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IBuiltInActuator.generated.h"

UENUM(BlueprintType)
enum class EBuiltInActuatorType : uint8
{
    Unknown UMETA(DisplayName = "Unknown"),
    AgentVectorActuator UMETA(DisplayName = "Agent Vector Actuator"),
    VectorActuator UMETA(DisplayName = "Vector Actuator"),
};

UINTERFACE(MinimalAPI, Blueprintable)
class UBuiltInActuator : public UInterface
{
    GENERATED_BODY()
};

class DRL_API IBuiltInActuator
{
    GENERATED_BODY()

public:
    /// <summary>
    /// Return the corresponding BuiltInActuatorType for the actuator.
    /// </summary>
    /// <returns>A BuiltInActuatorType corresponding to the actuator.</returns>
    virtual EBuiltInActuatorType GetBuiltInActuatorType() const = 0;
};