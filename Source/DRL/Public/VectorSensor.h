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
	UFUNCTION(BlueprintCallable, Category = "Observations")
	void AddFloatObservation(float Observation);

	UFUNCTION(BlueprintCallable, Category = "Observations")
	void AddIntObservation(int32 Observation);

	UFUNCTION(BlueprintCallable, Category = "Observations")
	void AddVectorObservation(FVector Observation);

	UFUNCTION(BlueprintCallable, Category = "Observations")
	void AddVector2DObservation(FVector2D Observation);

	UFUNCTION(BlueprintCallable, Category = "Observations")
	void AddFloatArrayObservation(const TArray<float>& Observation);

	UFUNCTION(BlueprintCallable, Category = "Observations")
	void AddQuatObservation(FQuat Observation);

	UFUNCTION(BlueprintCallable, Category = "Observations")
	void AddBoolObservation(bool Observation);

	UFUNCTION(BlueprintCallable, Category = "Observations")
    void AddOneHotObservation(int32 Observation, int32 Range);

private:
    void Clear();
    void AddFloatObs(float Obs);

    UPROPERTY()
    TArray<float> Observations;

    FString Name;
    FObservationSpec ObservationSpec;
};