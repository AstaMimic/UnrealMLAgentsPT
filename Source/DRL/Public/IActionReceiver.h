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
    FActionSegment<int> DiscreteActions;

    /** An empty action buffer. */
    static const FActionBuffers Empty;

    FActionBuffers()
        : ContinuousActions(), DiscreteActions()
    {
    }

    /** Construct an ActionBuffers instance with the continuous and discrete actions that will be used. */
    FActionBuffers(const FActionSegment<float>& InContinuousActions, const FActionSegment<int>& InDiscreteActions)
        : ContinuousActions(InContinuousActions), DiscreteActions(InDiscreteActions)
    {
    }

    /** Construct an ActionBuffers instance with the continuous and discrete actions that will be used. */
    FActionBuffers(const TArray<float>& InContinuousActions, const TArray<int32>& InDiscreteActions)
        : ContinuousActions(InContinuousActions), DiscreteActions(InDiscreteActions)
    {
    }

    /** Construct an ActionBuffers instance with ActionSpec. All values are initialized to zeros. */
    FActionBuffers(const FActionSpec& ActionSpec)
    {
        TArray<float> ContinuousActionArray;
        ContinuousActionArray.SetNumZeroed(ActionSpec.NumContinuousActions);
        ContinuousActions = FActionSegment<float>(ContinuousActionArray);

        TArray<int32> DiscreteActionArray;
        DiscreteActionArray.SetNumZeroed(ActionSpec.GetNumDiscreteActions());
        DiscreteActions = FActionSegment<int32>(DiscreteActionArray);
    }

    /** Create an ActionBuffers instance with ActionSpec and all actions stored as a float array. */
    static FActionBuffers FromActionSpec(const FActionSpec& ActionSpec, const TArray<float>& Actions)
    {
        if (Actions.Num() == 0)
        {
            return FActionBuffers::Empty;
        }

        ensure(Actions.Num() == ActionSpec.NumContinuousActions + ActionSpec.GetNumDiscreteActions());

        FActionSegment<float> ContinuousActionSegment;
        FActionSegment<int32> DiscreteActionSegment;
        int32 Offset = 0;

        if (ActionSpec.NumContinuousActions > 0)
        {
            TArray<float> ContinuousActionArray;
            ContinuousActionArray.Append(Actions.GetData(), ActionSpec.NumContinuousActions);
            ContinuousActionSegment = FActionSegment<float>(ContinuousActionArray);
            Offset += ActionSpec.NumContinuousActions;
        }

        if (ActionSpec.GetNumDiscreteActions() > 0)
        {
            TArray<int32> DiscreteActions;
            for (int32 i = 0; i < ActionSpec.GetNumDiscreteActions(); ++i)
            {
                DiscreteActions.Add(static_cast<int32>(Actions[Offset + i]));
            }
            DiscreteActionSegment = FActionSegment<int32>(DiscreteActions);
        }

        return FActionBuffers(ContinuousActionSegment, DiscreteActionSegment);
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