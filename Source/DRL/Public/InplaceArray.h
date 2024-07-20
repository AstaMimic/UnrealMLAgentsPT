#pragma once

#include "CoreMinimal.h"

template <typename T>
struct FInplaceArray
{
private:
    static const int32 MaxLength = 4;
    int32 Length;
    T Elem0;
    T Elem1;
    T Elem2;
    T Elem3;

public:
    FInplaceArray()
        : Length(0), Elem0(T()), Elem1(T()), Elem2(T()), Elem3(T())
    {
    }

    FInplaceArray(T InElem0)
        : Length(1), Elem0(InElem0), Elem1(T()), Elem2(T()), Elem3(T())
    {
    }

    FInplaceArray(T InElem0, T InElem1)
        : Length(2), Elem0(InElem0), Elem1(InElem1), Elem2(T()), Elem3(T())
    {
    }

    FInplaceArray(T InElem0, T InElem1, T InElem2)
        : Length(3), Elem0(InElem0), Elem1(InElem1), Elem2(InElem2), Elem3(T())
    {
    }

    FInplaceArray(T InElem0, T InElem1, T InElem2, T InElem3)
        : Length(4), Elem0(InElem0), Elem1(InElem1), Elem2(InElem2), Elem3(InElem3)
    {
    }

    int32 GetLength() const
    {
        return Length;
    }

    T& operator[](int32 Index)
    {
        check(Index >= 0 && Index < Length);
        switch (Index)
        {
        case 0: return Elem0;
        case 1: return Elem1;
        case 2: return Elem2;
        case 3: return Elem3;
        default: 
            checkNoEntry();
            return Elem0; // To satisfy the compiler
        }
    }

    const T& operator[](int32 Index) const
    {
        check(Index >= 0 && Index < Length);
        switch (Index)
        {
        case 0: return Elem0;
        case 1: return Elem1;
        case 2: return Elem2;
        case 3: return Elem3;
        default:
            checkNoEntry();
            return Elem0; // To satisfy the compiler
        }
    }

    static FInplaceArray<T> FromArray(const TArray<T>& Elems)
    {
        switch (Elems.Num())
        {
        case 1: return FInplaceArray<T>(Elems[0]);
        case 2: return FInplaceArray<T>(Elems[0], Elems[1]);
        case 3: return FInplaceArray<T>(Elems[0], Elems[1], Elems[2]);
        case 4: return FInplaceArray<T>(Elems[0], Elems[1], Elems[2], Elems[3]);
        default:
            UE_LOG(LogTemp, Error, TEXT("Array must have between 1 and 4 elements"));
            return FInplaceArray<T>();
        }
    }
};