#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionSegment.h"
#include "ActionSpec.h"
#include "IDiscreteActionMask.h"
#include "IActionReceiver.generated.h"

/**
 * A structure that wraps the ActionSegment for a particular IActionReceiver
 * and is used when IActionReceiver.OnActionReceived is called.
 */
USTRUCT(BlueprintType)
struct FActionBuffers
{
    GENERATED_BODY()

public:
    /** Holds the Continuous ActionSegment to be used by an IActionReceiver. */
    FActionSegment<float> ContinuousActions;

    /** Holds the Discrete ActionSegment to be used by an IActionReceiver. */
    FActionSegment<int32> DiscreteActions;

    /** An empty action buffer. */
    static FActionBuffers Empty;

    /** Construct an ActionBuffers instance with the continuous and discrete actions that will be used. */
    FActionBuffers()
        : ContinuousActions(nullptr), DiscreteActions(nullptr)
    {
    }

    /** Construct an ActionBuffers instance with the continuous and discrete actions that will be used. */
    FActionBuffers(FActionSegment<float> InContinuousActions, FActionSegment<int32> InDiscreteActions)
        : ContinuousActions(InContinuousActions), DiscreteActions(InDiscreteActions)
    {
    }

    /** Construct an ActionBuffers instance with the continuous and discrete actions that will be used. */
    FActionBuffers(TSharedPtr<TArray<float>> InContinuousActions, TSharedPtr<TArray<int32>> InDiscreteActions)
        : FActionBuffers(FActionSegment<float>(InContinuousActions), FActionSegment<int32>(InDiscreteActions))
    {
    }

    /** Clear the ContinuousActions and DiscreteActions segments to be all zeros. */
    void Clear()
    {
        ContinuousActions.Clear();
        DiscreteActions.Clear();
    }

    /** Check if the ActionBuffers is empty. */
    bool IsEmpty() const
    {
        return ContinuousActions.IsEmpty() && DiscreteActions.IsEmpty();
    }

    /** Equality operator */
    bool operator==(const FActionBuffers& Other) const
    {
        return ContinuousActions == Other.ContinuousActions && DiscreteActions == Other.DiscreteActions;
    }

    /** Inequality operator */
    bool operator!=(const FActionBuffers& Other) const
    {
        return !(*this == Other);
    }
};

/**
 * An interface that describes an object that can receive actions from a Reinforcement Learning network.
 */
UINTERFACE(MinimalAPI)
class UActionReceiver : public UInterface
{
    GENERATED_BODY()
};

class DRL_API IActionReceiver
{
    GENERATED_BODY()

public:
    /** Method called to execute actions based on the ActionBuffers contents. */
    virtual void OnActionReceived(const FActionBuffers& ActionBuffers) = 0;

    /** Modify the masks for discrete actions. */
    virtual void WriteDiscreteActionMask(const TScriptInterface<IDiscreteActionMask>& ActionMask) = 0;
};