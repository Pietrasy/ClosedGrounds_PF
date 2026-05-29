#pragma once

#include "FCG_DaySaveData.generated.h"

USTRUCT()
struct FCG_DaySaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 CurrentDay = 0;
};
