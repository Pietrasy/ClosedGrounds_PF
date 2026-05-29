#pragma once
#include "FCG_ObjectiveState.h"
#include "Data/ECG_Region.h"
#include "FCG_QuestState.generated.h"

class UCG_QuestData;

USTRUCT(BlueprintType)
struct FCG_QuestState
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Quest")
	FName QuestName;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Quest")
	TArray<FCG_ObjectiveState> QuestObjectives;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Quest")
	ERegion Region = ERegion::Null;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Quest")
	FGameplayTag QuestTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Quest")
	bool bIsCompleted = false;
	
	bool IsCompleted()
	{
		bool bAreAllObjectiveCompleted = true;
		
		for (const FCG_ObjectiveState& Objective : QuestObjectives)
		{
			if (!Objective.IsCompleted())
			{
				bAreAllObjectiveCompleted = false;
			}
		}
		return bIsCompleted = bAreAllObjectiveCompleted;
	}

	FCG_ObjectiveState* GetQuestObjective(const int32 InObjectiveIndex)
	{
		if (QuestObjectives.IsValidIndex(InObjectiveIndex))
		{
			return &QuestObjectives[InObjectiveIndex];
		}
		
		return nullptr;
	}
};
