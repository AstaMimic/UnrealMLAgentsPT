#include "RaySensor.h"

void URaySensor::Initialize(FString Name, FRayInput& RayInput) {
	_Name = Name;
    _RayInput = RayInput;
}

/// <inheritdoc/>
EBuiltInSensorType URaySensor::GetBuiltInSensorType() const
{
	return EBuiltInSensorType::RaySensor;
}

FObservationSpec URaySensor::GetObservationSpec() {
	return _ObservationSpec;
}

int32 URaySensor::Write(ObservationWriter& Writer) {
    _Observations.Empty(_Observations.Num());

    // Write all the logic to update observations based on HitResults
    for (const FHitResult& HitResult : _HitResults)
    {
        float ActorHash;
        AActor* HitActor = HitResult.GetActor();
        ActorHash = GetActorHash(HitActor);
        _Observations.Add(HitResult.Distance);
        _Observations.Add(ActorHash);
    }

    Writer.AddList(_Observations);
    return _Observations.Num();
}

void URaySensor::Update() {
    PerformRaycasts();
}

void URaySensor::Reset() {

}

FString URaySensor::GetName() const {
	return _Name;
}

void URaySensor::PerformRaycasts()
{
    // Clean _HitResuls before performing new RayCast
    _HitResults.Empty();

    const FVector Origin = _RayInput.Origin;
    const FQuat Rotation = _RayInput.Rotation;
    const FVector Forward = GetForwardVector();
    const FVector Right = FVector::CrossProduct(FVector::UpVector, Forward).GetSafeNormal();
    const FVector Up = FVector::CrossProduct(Forward, Right).GetSafeNormal();

    const int32 NumTheta = FMath::Sqrt((double)_RayInput.NumRays);  // Assume square root of NumRays for even grid
    const int32 NumPhi = NumTheta;

    for (int32 i = 0; i < NumTheta; ++i)
    {
        float Theta = (PI / 2) * i / (NumTheta - 1);  // Polar angle from 0 to PI/2
        for (int32 j = 0; j < NumPhi; ++j)
        {
            float Phi = PI * j / (NumPhi - 1);  // Azimuthal angle from 0 to PI
            FVector LocalDirection = FMath::Sin(Theta) * (FMath::Cos(Phi) * Right + FMath::Sin(Phi) * Up) + FMath::Cos(Theta) * Forward;
            FVector WorldDirection = Rotation.RotateVector(LocalDirection);
            FVector End = Origin + WorldDirection * _RayInput.RayLength;

            FHitResult HitResult;
            FCollisionQueryParams QueryParams;
            QueryParams.AddIgnoredActor(_RayInput.IgnoredActor);

            if (GetWorld()->LineTraceSingleByChannel(HitResult, Origin, End, ECC_Visibility, QueryParams))
            {
                _HitResults.Add(HitResult);
            }
            else
            {
                HitResult.Distance = _RayInput.RayLength;
                _HitResults.Add(HitResult);
            }
        }
    }
}


FVector URaySensor::GetForwardVector() const
{
    switch (_RayInput.ForwardAxis)
    {
        case EForwardAxis::X: return FVector(1, 0, 0);
        case EForwardAxis::NegativeX: return FVector(-1, 0, 0);
        case EForwardAxis::Y: return FVector(0, 1, 0);
        case EForwardAxis::NegativeY: return FVector(0, -1, 0);
        case EForwardAxis::Z: return FVector(0, 0, 1);
        case EForwardAxis::NegativeZ: return FVector(0, 0, -1);
        default: return FVector(1, 0, 0);
    }
}

float URaySensor::GetActorHash(AActor* Actor)
{
    if (!Actor)
    {
        return -1.0f;
    }

    // Get the object path
    FString ObjectPath = Actor->GetPathName();

    // Generate the MD5 hash
    FMD5 Md5;
    Md5.Update((const uint8*)TCHAR_TO_ANSI(*ObjectPath), ObjectPath.Len());
    uint8 Hash[16];
    Md5.Final(Hash);

    // Convert the hash to a hex string
    FString HashString = BytesToHex(Hash, 16);
    FString ShortHashString = HashString.Left(4);

    return HashToFloat(ShortHashString);
}

float URaySensor::HashToFloat(const FString& HashString)
{
    // Convert the first 8 characters of the hash string to an integer
    int32 IntValue = FCString::Strtoi(*HashString.Left(8), nullptr, 16);

    // Normalize the integer to a float between 0 and 1
    float NormalizedValue = static_cast<float>(IntValue) / static_cast<float>(0xFFFFFFFF);

    return NormalizedValue;
}
