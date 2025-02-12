// Copyright © 2025 Stephane Capponi and individual contributors. All Rights Reserved.

#include "UnrealMLAgents/Sensors/SensorComponent.h"

TArray<TScriptInterface<IISensor>> USensorComponent::CreateSensors_Implementation()
{
	// Derived classes should override this method to create specific sensors
	return TArray<TScriptInterface<IISensor>>();
}
