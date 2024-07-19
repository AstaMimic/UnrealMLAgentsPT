#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ISensor.h"
#include "SensorComponent.generated.h"

UCLASS(Abstract, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SENSORS_API USensorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // This function should be overridden in derived classes to create sensor instances
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sensors")
    TArray<TScriptInterface<IISensor>> CreateSensors();
    virtual TArray<TScriptInterface<IISensor>> CreateSensors_Implementation();
};