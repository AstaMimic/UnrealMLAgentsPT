#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionSpec.h"
#include "IDiscreteActionMask.h"
#include "IActionReceiver.h"
#include "IActuator.generated.h"

/**
 * Abstraction that facilitates the execution of actions.
 */
UINTERFACE(MinimalAPI)
class UActuator : public UInterface
{
    GENERATED_BODY()
};

class IActuator
{
    GENERATED_BODY()

public:
    /**
     * The specification of the actions for this IActuator.
     */
    virtual FActionSpec GetActionSpec() const = 0;

    /**
     * Gets the name of this IActuator which will be used to sort it.
     */
    virtual FString GetName() const = 0;

    /**
     * Resets the internal state of the actuator. This is called at the end of an Agent's episode.
     * Most implementations can leave this empty.
     */
    virtual void ResetData() = 0;

    virtual void Heuristic(const FActionBuffers& ActionBuffersOut) = 0;

    /** Method called to execute actions based on the ActionBuffers contents. */
    virtual void OnActionReceived(const FActionBuffers& ActionBuffers) = 0;

    /** Modify the masks for discrete actions. */
    virtual void WriteDiscreteActionMask(IDiscreteActionMask* ActionMask) = 0;
};

/**
 * Helper methods to be shared by all classes that implement IActuator.
 */
class IActuatorExtensions
{
public:
    /**
     * Returns the number of discrete branches + the number of continuous actions.
     */
    static int32 TotalNumberOfActions(const IActuator& Actuator)
    {
        return Actuator.GetActionSpec().NumContinuousActions + Actuator.GetActionSpec().GetNumDiscreteActions();
    }
};