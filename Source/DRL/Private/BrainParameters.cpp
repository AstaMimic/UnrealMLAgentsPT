#include "BrainParameters.h"

UBrainParameters* UBrainParameters::Clone() const
{
    UBrainParameters* NewBrainParameters = NewObject<UBrainParameters>();
    NewBrainParameters->VectorObservationSize = VectorObservationSize;
    NewBrainParameters->NumStackedVectorObservations = NumStackedVectorObservations;
    NewBrainParameters->VectorActionDescriptions = VectorActionDescriptions;
    NewBrainParameters->ActionSpec = FActionSpec(ActionSpec.NumContinuousActions, ActionSpec.BranchSizes);
    NewBrainParameters->bHasUpgradedBrainParametersWithActionSpec = bHasUpgradedBrainParametersWithActionSpec;
    return NewBrainParameters;
}