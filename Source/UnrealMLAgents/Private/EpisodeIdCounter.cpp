// Copyright © 2025 Stephane Capponi and individual contributors. All Rights Reserved.

#include "UnrealMLAgents/EpisodeIdCounter.h"

int32 EpisodeIdCounter::Counter = 0;

int32 EpisodeIdCounter::GetEpisodeId()
{
	return Counter++;
}
