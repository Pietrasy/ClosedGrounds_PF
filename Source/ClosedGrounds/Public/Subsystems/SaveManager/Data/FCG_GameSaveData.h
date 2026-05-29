#pragma once

#include "FCG_GameSaveData.generated.h"

USTRUCT()
struct FCG_GameSaveData
{
	GENERATED_BODY();
	
	UPROPERTY()
	int32 CorruptionLevel = 0;
};
