#pragma once

#include "CoreMinimal.h"

template <typename T>
struct FActionSegment
{

public:
    int32 Offset;
    int32 Length;
    TArray<T> Array;

    /** An empty action segment. */
    static const FActionSegment<T> Empty;

    // Default constructor
    FActionSegment()
        : Offset(0), Length(0)
    {
    }

    // Constructor with array, offset, and length
    FActionSegment(const TArray<T>& InArray, int32 InOffset, int32 InLength)
        : Offset(InOffset), Length(InLength), Array(InArray)
    {
        CheckParameters(InArray, InOffset, InLength);
    }

    /** Constructor with just an array */
    FActionSegment(const TArray<T>& InArray)
        : Offset(0), Length(InArray.Num()), Array(InArray)
    {
    }


    // Check the parameters for validity
    static void CheckParameters(const TArray<T>& InArray, int32 InOffset, int32 InLength)
    {
        if (InOffset + InLength > InArray.Num())
        {
            UE_LOG(LogTemp, Error, TEXT("Arguments offset: %d and length: %d are out of bounds of array: %d."), InOffset, InLength, InArray.Num());
        }
    }

    // Index operator
    T operator[](int32 Index) const
    {
        if (Index < 0 || Index >= Length)
        {
            UE_LOG(LogTemp, Error, TEXT("Index out of bounds, expected a number between 0 and %d"), Length - 1);
            return T(); // Return a default value or handle the error as needed
        }
        return Array[Offset + Index];
    }

    T& operator[](int32 Index)
    {
        if (Index < 0 || Index >= Length)
        {
            UE_LOG(LogTemp, Error, TEXT("Index out of bounds, expected a number between 0 and %d"), Length - 1);
            static T DefaultValue = T(); // Return a default value reference or handle the error as needed
            return DefaultValue;
        }
        return Array[Offset + Index];
    }

    // Clear the segment
    void Clear()
    {
        for (int32 i = 0; i < Length; ++i)
        {
            Array[Offset + i] = T();
        }
    }

    // Check if the segment is empty
    bool IsEmpty() const
    {
        return Length == 0;
    }

    // Equality operator
    bool operator==(const FActionSegment& Other) const
    {
        if (Offset != Other.Offset || Length != Other.Length)
        {
            return false;
        }
        for (int32 i = 0; i < Length; ++i)
        {
            if (Array[Offset + i] != Other.Array[Other.Offset + i])
            {
                return false;
            }
        }
        return true;
    }

    // Inequality operator
    bool operator!=(const FActionSegment& Other) const
    {
        return !(*this == Other);
    }
};

template<typename T>
const FActionSegment<T> FActionSegment<T>::Empty = FActionSegment<T>(TArray<T>(), 0, 0);

