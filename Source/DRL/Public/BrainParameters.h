#pragma once

#include "CoreMinimal.h"
#include "ActionSpec.h"
#include "BrainParameters.generated.h"

USTRUCT(BlueprintType)
struct DRL_API FBrainParameters
{
    GENERATED_BODY()

public:
    // The number of the observations that are added in
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brain Parameters")
    int32 VectorObservationSize = 1;

    // Stacking refers to concatenating the observations across multiple frames. This field
    // indicates the number of frames to concatenate across.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brain Parameters", meta = (ClampMin = "1", ClampMax = "50"))
    int32 NumStackedVectorObservations = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brain Parameters")
    FActionSpec ActionSpec;

    // The list of strings describing what the actions correspond to.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brain Parameters")
    TArray<FString> VectorActionDescriptions;

    bool bHasUpgradedBrainParametersWithActionSpec;

    // Method to clone this struct
    FBrainParameters Clone() const
    {
        FBrainParameters ClonedStruct;
        ClonedStruct.VectorObservationSize = this->VectorObservationSize;
        ClonedStruct.NumStackedVectorObservations = this->NumStackedVectorObservations;
        ClonedStruct.ActionSpec = this->ActionSpec;
        ClonedStruct.VectorActionDescriptions = this->VectorActionDescriptions;
        ClonedStruct.bHasUpgradedBrainParametersWithActionSpec = this->bHasUpgradedBrainParametersWithActionSpec;
        return ClonedStruct;
    }
};