#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "FCG_QuestObjective.generated.h"

struct FCG_ActiveQuest;

UENUM(BlueprintType, Blueprintable)
enum class EQuestObjectiveType : uint8
{
	Kill,
	Destroy,
	Collect,
	Interact,
	Default
};

USTRUCT(BlueprintType)
struct FCG_QuestObjective
{
	GENERATED_BODY()
	
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	//EQuestObjectiveType Type = EQuestObjectiveType::Default;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag ObjectiveTag = FGameplayTag::EmptyTag;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 RequiredAmount = 1;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 CurrentAmount = 0;
	
	FCG_ActiveQuest* OwningQuest = nullptr;
	
	bool IsCompleted() const { return CurrentAmount >= RequiredAmount; }
};
