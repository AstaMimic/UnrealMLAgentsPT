#include "UnrealMLAgents/Actuators/ActionSegment.h"

// Static member initialization
template <typename T>
FActionSegment<T> FActionSegment<T>::Empty = FActionSegment<T>(FActionSegment<T>::GetEmptyArray(), 0, 0);

// Explicit instantiation for required types
template struct FActionSegment<float>;
template struct FActionSegment<int>;
