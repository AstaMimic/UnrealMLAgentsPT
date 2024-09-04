#pragma once

#include "CoreMinimal.h"

template <typename T>
struct FActionSegment
{

public:
    int32 Offset;
    int32 Length;
    TSharedPtr<TArray<T>> Array;

    // Static member for an empty ActionSegment
    static FActionSegment<T> Empty;

    // Constructor with just an array
    FActionSegment(TSharedPtr<TArray<T>> InArray)
        : FActionSegment(InArray.IsValid() ? InArray : FActionSegment<T>::GetEmptyArray(), 0, InArray.IsValid() ? InArray->Num() : 0)
    {
    }

    // Constructor with array, offset, and length
    FActionSegment(TSharedPtr<TArray<T>> InArray, int32 InOffset, int32 InLength)
        : Offset(InOffset), Length(InLength), Array(InArray.IsValid() ? InArray : GetEmptyArray())
    {
        CheckParameters(*Array, InOffset, InLength);
    }

    // Static function to return an empty array
    static TSharedPtr<TArray<T>> GetEmptyArray()
    {
        static TSharedPtr<TArray<T>> EmptyArray = MakeShared<TArray<T>>();
        return EmptyArray;
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
        return (*Array)[Offset + Index];
    }

    T& operator[](int32 Index)
    {
        if (Index < 0 || Index >= Length)
        {
            UE_LOG(LogTemp, Error, TEXT("Index out of bounds, expected a number between 0 and %d"), Length - 1);
            static T DefaultValue = T(); // Return a default value reference or handle the error as needed
            return DefaultValue;
        }
        return (*Array)[Offset + Index];
    }

    void Clear()
    {
        if (Array.IsValid())
        {
            TArray<T>& ArrayRef = *Array;  // Dereference the pointer to access the array

            // Ensure we don't go out of bounds
            if (Offset + Length <= ArrayRef.Num())
            {
                for (int32 i = Offset; i < Offset + Length; ++i)
                {
                    ArrayRef[i] = T();  // Default-initialize each element (for POD types like int, float) 
                                        // or call the default constructor for classes/structs
                }
            }
        }
    }

    // Check if the segment is empty
    bool IsEmpty() const
    {
        return Array == nullptr || Array->Num() == 0;
    }

    // Equality operator
    bool operator==(const FActionSegment& Other) const
    {
        return Offset == Other.Offset && Length == Other.Length && Array == Other.Array;
    }

    // Inequality operator
    bool operator!=(const FActionSegment& Other) const
    {
        return !(*this == Other);
    }
};
