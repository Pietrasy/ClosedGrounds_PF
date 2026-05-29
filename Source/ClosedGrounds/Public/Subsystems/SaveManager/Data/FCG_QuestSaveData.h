#pragma once

#include "FCG_QuestSaveData.generated.h"

USTRUCT()
struct FCG_QuestSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FPrimaryAssetId> ActiveQuests;
	
	UPROPERTY()
	TArray<FPrimaryAssetId> RemainingQuests;
};
