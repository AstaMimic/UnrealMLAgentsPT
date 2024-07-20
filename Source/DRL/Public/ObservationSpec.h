#pragma once

#include "CoreMinimal.h"
#include "InplaceArray.h"
#include "DimensionProperty.h"
#include "ObservationType.h"
#include "ObservationSpec.generated.h"


USTRUCT(BlueprintType)
struct DRL_API FObservationSpec
{
    GENERATED_BODY()

private:
    FInplaceArray<int32> Shape;
    FInplaceArray<EDimensionProperty> DimensionProperties;
    EObservationType ObservationType;

public:
    FObservationSpec() {}

    FObservationSpec(FInplaceArray<int32> InShape, FInplaceArray<EDimensionProperty> InDimensionProperties, EObservationType InObservationType = EObservationType::Default)
        : Shape(InShape), DimensionProperties(InDimensionProperties), ObservationType(InObservationType)
    {
        check(Shape.GetLength() == DimensionProperties.GetLength());
    }

    const FInplaceArray<int32>& GetShape() const
    {
        return Shape;
    }

    const FInplaceArray<EDimensionProperty>& GetDimensionProperties() const
    {
        return DimensionProperties;
    }

    EObservationType GetObservationType() const
    {
        return ObservationType;
    }

    int32 GetRank() const
    {
        return Shape.GetLength();
    }

    static FObservationSpec Vector(int32 Length, EObservationType InObservationType = EObservationType::Default)
    {
        return FObservationSpec(FInplaceArray<int32>(Length), FInplaceArray<EDimensionProperty>(EDimensionProperty::None), InObservationType);
    }

    static FObservationSpec VariableLength(int32 ObsSize, int32 MaxNumObs)
    {
        return FObservationSpec(FInplaceArray<int32>(ObsSize, MaxNumObs), FInplaceArray<EDimensionProperty>(EDimensionProperty::VariableSize, EDimensionProperty::None));
    }

    static FObservationSpec Visual(int32 Channels, int32 Height, int32 Width, EObservationType InObservationType = EObservationType::Default)
    {
        return FObservationSpec(FInplaceArray<int32>(Channels, Height, Width), FInplaceArray<EDimensionProperty>(EDimensionProperty::None, EDimensionProperty::TranslationalEquivariance, EDimensionProperty::TranslationalEquivariance), InObservationType);
    }
};