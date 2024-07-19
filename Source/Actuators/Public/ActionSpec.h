#pragma once

#include "CoreMinimal.h"
#include "ActionSpec.generated.h"

/**
 * Defines the structure of the actions to be used by the Actuator system.
 */
USTRUCT(BlueprintType)
struct FActionSpec
{
    GENERATED_BODY()

public:
    /** Number of continuous actions. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Spec")
    int32 NumContinuousActions;

    /** Array of branch sizes for discrete actions. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Spec")
    TArray<int32> BranchSizes;

    /** Get the number of discrete actions */
    int32 GetNumDiscreteActions() const
    {
        return BranchSizes.Num();
    }

        /** Property for the number of discrete actions */
    UPROPERTY(BlueprintReadOnly, Category = "Action Spec")
    int32 NumDiscreteActions;

    /** Called after the struct is serialized to ensure properties are set correctly */
    void PostSerialize(const FArchive& Ar)
    {
        NumDiscreteActions = GetNumDiscreteActions();
    }

    /** Default constructor */
    FActionSpec()
        : NumContinuousActions(0)
    {
    }

    /** Constructor with parameters */
    FActionSpec(int32 InNumContinuousActions, const TArray<int32>& InBranchSizes)
        : NumContinuousActions(InNumContinuousActions), BranchSizes(InBranchSizes)
    {
    }

    /** Creates a Continuous ActionSpec with the number of actions available. */
    static FActionSpec MakeContinuous(int32 NumActions)
    {
        return FActionSpec(NumActions, TArray<int32>());
    }

    /** Creates a Discrete ActionSpec with the array of branch sizes. */
    static FActionSpec MakeDiscrete(const TArray<int32>& InBranchSizes)
    {
        return FActionSpec(0, InBranchSizes);
    }

    /** Checks that the ActionSpec uses either all continuous or all discrete actions. */
    void CheckAllContinuousOrDiscrete() const
    {
        if (NumContinuousActions > 0 && BranchSizes.Num() > 0)
        {
            UE_LOG(LogTemp, Error, TEXT("Action spaces with both continuous and discrete actions are not supported. " \
                "ActionSpecs must be all continuous or all discrete."));
        }
    }

    /** Combines a list of action specs into a single ActionSpec. */
    static FActionSpec Combine(const TArray<FActionSpec>& Specs)
    {
        int32 TotalContinuous = 0;
        int32 TotalDiscrete = 0;
        for (const FActionSpec& Spec : Specs)
        {
            TotalContinuous += Spec.NumContinuousActions;
            TotalDiscrete += Spec.BranchSizes.Num();
        }

        if (TotalDiscrete <= 0)
        {
            return MakeContinuous(TotalContinuous);
        }

        TArray<int32> CombinedBranchSizes;
        CombinedBranchSizes.SetNumUninitialized(TotalDiscrete);
        int32 Offset = 0;
        for (const FActionSpec& Spec : Specs)
        {
            if (Spec.BranchSizes.Num() > 0)
            {
                const int32 BranchSizeCount = Spec.BranchSizes.Num();
                FMemory::Memcpy(CombinedBranchSizes.GetData() + Offset, Spec.BranchSizes.GetData(), BranchSizeCount * sizeof(int32));
                Offset += BranchSizeCount;
            }
        }

        return FActionSpec(TotalContinuous, CombinedBranchSizes);
    }

    /** Returns the total number of discrete actions. */
    int32 GetSumOfDiscreteBranchSizes() const
    {
        int32 Sum = 0;
        for (int32 BranchSize : BranchSizes)
        {
            Sum += BranchSize;
        }
        return Sum;
    }
    
};

