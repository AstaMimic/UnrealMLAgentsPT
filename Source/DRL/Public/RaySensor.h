#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ISensor.h"
#include "IBuiltinSensor.h"
#include "RaySensor.generated.h"


UENUM(BlueprintType)
enum class EForwardAxis : uint8
{
    X = 0,
    NegativeX,
    Y,
    NegativeY,
    Z,
    NegativeZ
};


USTRUCT(BlueprintType)
struct DRL_API FRayInput
{
    GENERATED_BODY()

public:
    // The starting point of the ray
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ray Perception")
    FVector Origin;

    // The direction of the ray
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ray Perception")
    FQuat Rotation;

    // The length of the ray
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ray Perception")
    float RayLength;

    // The length of the ray
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ray Perception")
    int32 NumRays;

    // The axis
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Raycast")
    EForwardAxis ForwardAxis;

    // Ignore character
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Raycast")
    AActor* IgnoredActor;


    // Constructor
    FRayInput()
        : Origin(FVector::ZeroVector)
        , Rotation(FQuat::Identity)
        , RayLength(1000.0f)
        , NumRays(10)
        , ForwardAxis(EForwardAxis::X)
        , IgnoredActor(nullptr)
    {
    }

};


UCLASS(Blueprintable)
class DRL_API URaySensor : public UObject, public IISensor, public IBuiltInSensor
{
    GENERATED_BODY()

public:

    void Initialize(FString Name, FRayInput& RayInput);

    // IISensor

    virtual FObservationSpec GetObservationSpec() override;
    virtual int32 Write(ObservationWriter& Writer) override;
    virtual void Update() override;
    virtual void Reset() override;
    virtual FString GetName() const override;

    // IBuildItSensor

    virtual EBuiltInSensorType GetBuiltInSensorType() const override;

private:
    
    void PerformRaycasts();
    FVector GetForwardVector() const;
    float GetActorHash(AActor* Actor);
    float HashToFloat(const FString& HashString);

    FObservationSpec _ObservationSpec;
    TArray<float> _Observations;
    FString _Name;
    TArray<FHitResult> _HitResults;
    FRayInput _RayInput;
};