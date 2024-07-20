#pragma once

#include "CoreMinimal.h"
#include "ActionSpec.h"
#include "BrainParameters.generated.h"

UCLASS(Blueprintable)
class DRL_API UBrainParameters : public UObject
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

    UPROPERTY()
    bool bHasUpgradedBrainParametersWithActionSpec;

    // Deep clones the BrainParameter object.
    UFUNCTION(BlueprintCallable, Category = "Brain Parameters")
    UBrainParameters* Clone() const;

};