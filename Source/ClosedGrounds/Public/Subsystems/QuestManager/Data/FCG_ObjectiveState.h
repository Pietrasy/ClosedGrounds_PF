#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "FCG_ObjectiveState.generated.h"

struct FCG_QuestState;

USTRUCT(BlueprintType)
struct FCG_ObjectiveState
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag ObjectiveTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 RequiredProgress = 1;

	int32 CurrentProgress = 0;
	
	bool IsCompleted() const { return CurrentProgress >= RequiredProgress; }
	
	void IncrementProgress()
	{
		if (CurrentProgress < RequiredProgress)
		{
			++CurrentProgress;
		}
	}
};
