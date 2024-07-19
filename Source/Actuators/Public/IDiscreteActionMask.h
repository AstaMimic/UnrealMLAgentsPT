#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDiscreteActionMask.generated.h"

/**
 * Interface for writing a mask to disable discrete actions for agents for the next decision.
 */
UINTERFACE(MinimalAPI)
class UDiscreteActionMask : public UInterface
{
    GENERATED_BODY()
};

class IDiscreteActionMask
{
    GENERATED_BODY()

public:
    virtual void SetActionEnabled(int32 Branch, int32 ActionIndex, bool bIsEnabled) = 0;
};