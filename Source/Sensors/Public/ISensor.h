#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObservationSpec.h"
#include "ObservationType.h"
#include "DimensionProperty.h"
#include "ObservationWriter.h"
#include "ISensor.generated.h"


UINTERFACE(MinimalAPI)
class UISensor : public UInterface
{
    GENERATED_BODY()
};

class SENSORS_API IISensor
{
    GENERATED_BODY()

public:
    virtual FObservationSpec GetObservationSpec() = 0;
    virtual int32 Write(ObservationWriter& Writer) = 0;
    virtual void Update() = 0;
    virtual void Reset() = 0;
    virtual FString GetName() const = 0;
};


UCLASS()
class SENSORS_API USensorExtensions : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Sensor")
    static int32 ObservationSize(TScriptInterface<IISensor> Sensor)
    {
        FObservationSpec ObsSpec = Sensor->GetObservationSpec();
        int32 Count = 1;
        for (int32 i = 0; i < ObsSpec.GetRank(); ++i)
        {
            Count *= ObsSpec.GetShape()[i];
        }
        return Count;
    }
};


UCLASS()
class SENSORS_API USensorUtils : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Sensor")
    static void SortSensors(TArray<TScriptInterface<IISensor>>& Sensors)
    {
        Sensors.Sort([](const TScriptInterface<IISensor>& A, const TScriptInterface<IISensor>& B)
        {
            return A->GetName().Compare(B->GetName()) < 0;
        });
    }
};