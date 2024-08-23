#pragma once

#include "CoreMinimal.h"
#include "IActionReceiver.h"
#include "AgentInfo.generated.h"


USTRUCT(BlueprintType)
struct DRL_API FAgentInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    float Reward;

    UPROPERTY(BlueprintReadWrite)
    float GroupReward;

    UPROPERTY(BlueprintReadWrite)
    bool bDone;

    UPROPERTY(BlueprintReadWrite)
    bool bMaxStepReached;

    UPROPERTY(BlueprintReadWrite)
    int32 EpisodeId;

    UPROPERTY(BlueprintReadWrite)
    int32 GroupId;

    UPROPERTY(BlueprintReadWrite)
    FActionBuffers StoredActions;

    UPROPERTY(BlueprintReadWrite)
    TArray<bool> DiscreteActionMasks;

    void ClearActions() {
        StoredActions.Clear();
    }

    void CopyActions(FActionBuffers ActionBuffers) {
        FActionSegment<float>& ContinousAction = StoredActions.ContinuousActions;
        for (int i = 0; i < ActionBuffers.ContinuousActions.Length; i++) {
            ContinousAction[i] = ActionBuffers.ContinuousActions[i];;
        }
        FActionSegment<int32>& DiscreteAction = StoredActions.DiscreteActions;
        for (int i = 0; i < ActionBuffers.DiscreteActions.Length; i++) {
            DiscreteAction[i] = ActionBuffers.DiscreteActions[i];;
        }
    }

};
