#pragma once

#include "CoreMinimal.h" // Add this line

class EpisodeIdCounter
{
public:
	static int32 GetEpisodeId();

private:
	static int32 Counter;
};
