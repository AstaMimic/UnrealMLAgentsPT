#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHeuristicProvider.generated.h"

/**
 * Interface that allows objects to fill out an FActionBuffers data structure for controlling
 * behavior of Agents or Actuators.
 */
UINTERFACE(MinimalAPI)
class UHeuristicProvider : public UInterface
{
    GENERATED_BODY()
};

class IHeuristicProvider
{
    GENERATED_BODY()

public:
    virtual void Heuristic(const FActionBuffers& ActionBuffersOut) const = 0;
};

