#pragma once

#include "CoreMinimal.h"
#include "DimensionProperty.generated.h"

UENUM(BlueprintType)
enum class EDimensionProperty : uint8
{
    Unspecified = 0 UMETA(DisplayName = "Unspecified"),
    None = 1 UMETA(DisplayName = "None"),
    TranslationalEquivariance = 2 UMETA(DisplayName = "Translational Equivariance"),
    VariableSize = 4 UMETA(DisplayName = "Variable Size")
};