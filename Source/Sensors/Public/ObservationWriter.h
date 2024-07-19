// ObservationWriter.h
#pragma once

#include "CoreMinimal.h"
#include "google/protobuf/repeated_field.h"
#include "UObject/ObjectMacros.h"

class ObservationWriter
{
private:
    google::protobuf::RepeatedField<float>* Data;
    int Offset;
    int Batch;

    FInplaceArray<int32> Shape;

public:
    ObservationWriter() : Data(nullptr), Offset(0), Batch(0) {}

    void SetTarget(google::protobuf::RepeatedField<float>* InData, const FInplaceArray<int32>& InShape, int InOffset)
    {
        Data = InData;
        Shape = InShape;
        Offset = InOffset;
        Batch = 0;
    }

    float& operator[](int Index)
    {
        check(Data != nullptr);
        return (*Data)[Index + Offset];
    }

    float& operator()(int Ch, int W)
    {
        check(Data != nullptr);
        return (*Data)[Ch * Shape[Shape.GetLength() - 1] + W];
    }

    float& operator()(int Ch, int H, int W)
    {
        check(Data != nullptr);

        if (H < 0 || H >= Shape[1])
        {
            UE_LOG(LogTemp, Error, TEXT("Height value %d must be in range [0, %d]"), H, Shape[1] - 1);
            checkNoEntry();
        }

        if (W < 0 || W >= Shape[2])
        {
            UE_LOG(LogTemp, Error, TEXT("Width value %d must be in range [0, %d]"), W, Shape[2] - 1);
            checkNoEntry();
        }

        if (Ch < 0 || Ch >= Shape[0])
        {
            UE_LOG(LogTemp, Error, TEXT("Channel value %d must be in range [0, %d]"), Ch, Shape[0] - 1);
            checkNoEntry();
        }

        int Index = Ch * Shape[1] * Shape[2] + H * Shape[2] + W;
        return (*Data)[Index];
    }

    void AddList(const TArray<float>& InData, int WriteOffset = 0)
    {
        check(Data != nullptr);

        for (int Index = 0; Index < InData.Num(); Index++)
        {
            (*Data)[Index + Offset + WriteOffset] = InData[Index];
        }
    }

    void Add(const FVector& Vec, int WriteOffset = 0)
    {
        check(Data != nullptr);

        (*Data)[Offset + WriteOffset + 0] = Vec.X;
        (*Data)[Offset + WriteOffset + 1] = Vec.Y;
        (*Data)[Offset + WriteOffset + 2] = Vec.Z;
    }

    void Add(const FVector4& Vec, int WriteOffset = 0)
    {
        check(Data != nullptr);

        (*Data)[Offset + WriteOffset + 0] = Vec.X;
        (*Data)[Offset + WriteOffset + 1] = Vec.Y;
        (*Data)[Offset + WriteOffset + 2] = Vec.Z;
        (*Data)[Offset + WriteOffset + 3] = Vec.W;
    }

    void Add(const FQuat& Quat, int WriteOffset = 0)
    {
        check(Data != nullptr);

        (*Data)[Offset + WriteOffset + 0] = Quat.X;
        (*Data)[Offset + WriteOffset + 1] = Quat.Y;
        (*Data)[Offset + WriteOffset + 2] = Quat.Z;
        (*Data)[Offset + WriteOffset + 3] = Quat.W;
    }
};
