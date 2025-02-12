// Copyright © 2025 Stephane Capponi and individual contributors. All Rights Reserved.

#include "UnrealMLAgents/Actuators/IActionReceiver.h"

FActionBuffers FActionBuffers::Empty = FActionBuffers(FActionSegment<float>::Empty, FActionSegment<int32>::Empty);
