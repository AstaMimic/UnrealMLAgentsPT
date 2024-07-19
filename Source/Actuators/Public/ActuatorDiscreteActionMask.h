#pragma once

#include "CoreMinimal.h"
#include "IActuator.h"
#include "IDiscreteActionMask.h"
#include "ActuatorDiscreteActionMask.generated.h"

UCLASS()
class ACTUATORS_API UActuatorDiscreteActionMask : public UObject, public IDiscreteActionMask
{
    GENERATED_BODY()

public:
    int32 CurrentBranchOffset;

    UActuatorDiscreteActionMask()
    {
    }

	void Initialize(const TArray<TScriptInterface<IActuator>>& InActuators, int32 InSumOfDiscreteBranchSizes, int32 InNumDiscreteBranches, const TArray<int32>& InBranchSizes = TArray<int32>())
	{
		Actuators = InActuators;
		SumOfDiscreteBranchSizes = InSumOfDiscreteBranchSizes;
		NumBranches = InNumDiscreteBranches;
		BranchSizes = InBranchSizes;
	}

    void SetActionEnabled(int32 Branch, int32 ActionIndex, bool bIsEnabled) {
        LazyInitialize();
        CurrentMask[ActionIndex + StartingActionIndices[CurrentBranchOffset + Branch]] = !bIsEnabled;
    }

    TArray<bool> GetMask() const
    {
        return CurrentMask;
    }

    void ResetMask()
    {
        if (CurrentMask.Num() > 0)
        {
            FMemory::Memset(CurrentMask.GetData(), 0, CurrentMask.Num() * sizeof(bool));
        }
    }

private:
    TArray<TScriptInterface<IActuator>> Actuators;
    TArray<int32> StartingActionIndices;
    TArray<int32> BranchSizes;
    TArray<bool> CurrentMask;
    int32 SumOfDiscreteBranchSizes;
    int32 NumBranches;

    void LazyInitialize()
    {
        if (BranchSizes.Num() == 0)
        {
            BranchSizes.SetNum(NumBranches);
            int32 Start = 0;
            for (const auto& Actuator : Actuators)
            {
                const TArray<int32>& CopyBranchSizes = Actuator->GetActionSpec().BranchSizes;
                for (int32 Size : CopyBranchSizes)
                {
                    BranchSizes[Start++] = Size;
                }
            }
        }

        if (CurrentMask.Num() == 0)
        {
            CurrentMask.SetNum(SumOfDiscreteBranchSizes);
        }

        if (StartingActionIndices.Num() == 0)
        {
            StartingActionIndices = CumSum(BranchSizes);
        }
    }

    TArray<int32> CumSum(const TArray<int32>& InputArray)
    {
        TArray<int32> Result;
        Result.SetNum(InputArray.Num() + 1);
        Result[0] = 0;
        for (int32 i = 0; i < InputArray.Num(); ++i)
        {
            Result[i + 1] = Result[i] + InputArray[i];
        }
        return Result;
    }

    void AssertMask()
    {
        for (int32 BranchIndex = 0; BranchIndex < NumBranches; ++BranchIndex)
        {
            if (AreAllActionsMasked(BranchIndex))
            {
                UE_LOG(LogTemp, Error, TEXT("Invalid Action Masking: All the actions of branch %d are masked."), BranchIndex);
            }
        }
    }

    bool AreAllActionsMasked(int32 Branch)
    {
        if (CurrentMask.Num() == 0)
        {
            return false;
        }

        int32 Start = StartingActionIndices[Branch];
        int32 End = StartingActionIndices[Branch + 1];
        for (int32 i = Start; i < End; ++i)
        {
            if (!CurrentMask[i])
            {
                return false;
            }
        }

        return true;
    }
};
