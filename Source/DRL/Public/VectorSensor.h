#pragma once

#include "CoreMinimal.h"
#include "ISensor.h"
#include "ObservationWriter.h"
#include "IBuiltInSensor.h"
#include "VectorSensor.generated.h"


UCLASS(Blueprintable)
class DRL_API UVectorSensor : public UObject, public IISensor, public IBuiltInSensor
{
    GENERATED_BODY()

public:
    UVectorSensor();

    void Initialize(int32 ObservationSize, FString Name = "", EObservationType ObservationType = EObservationType::Default);

    // ISensor interface
    virtual int32 Write(ObservationWriter& Writer) override;
    virtual void Update() override;
    virtual void Reset() override;
    virtual FObservationSpec GetObservationSpec() override;
    virtual FString GetName() const override;
    virtual EBuiltInSensorType GetBuiltInSensorType() const override;

    // Compatibility methods with Agent observation
    void AddObservation(float Observation);
    void AddObservation(int32 Observation);
    void AddObservation(FVector Observation);
    void AddObservation(FVector2D Observation);
    void AddObservation(const TArray<float>& Observation);
    void AddObservation(FQuat Observation);
    void AddObservation(bool Observation);
    void AddOneHotObservation(int32 Observation, int32 Range);

private:
    void Clear();
    void AddFloatObs(float Obs);

    UPROPERTY()
    TArray<float> Observations;

    FString Name;
    FObservationSpec ObservationSpec;
};