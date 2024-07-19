#pragma once

#include "CoreMinimal.h"
#include "ObservationType.generated.h"

UENUM(BlueprintType)
enum class EObservationType : uint8
{
    Default = 0 UMETA(DisplayName = "Default"),
    GoalSignal = 1 UMETA(DisplayName = "Goal Signal")
};