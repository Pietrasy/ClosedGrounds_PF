#pragma once
#include "FCG_QuestObjective.h"
#include "FCG_ActiveQuest.generated.h"

class UCG_QuestData;

USTRUCT(BlueprintType)
struct FCG_ActiveQuest
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPrimaryAssetId QuestID;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCG_QuestData> QuestData;
	
	TArray<FCG_QuestObjective> Objectives;
	
	bool IsCompleted() const
	{
		for (const FCG_QuestObjective& TempObjective : Objectives)
		{
			if (!TempObjective.IsCompleted())
			return false;
		}
		return true;
	} 
};
