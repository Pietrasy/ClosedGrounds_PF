// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Data/FCG_QuestState.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CG_QuestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedTodayQuests);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadedAllQuests);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestSystemReady);
DECLARE_DELEGATE(FOnActivatedTodayQuests);

struct FObjectiveHandle
{
	int32 QuestIndex;
	int32 ObjectiveIndex;
};

class UCG_QuestData;

UCLASS()
class CLOSEDGROUNDS_API UCG_QuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void InitializeQuestSystem();
	
	UPROPERTY(BlueprintAssignable)
	FOnFinishedTodayQuests OnFinishedTodayQuests;
	FOnActivatedTodayQuests OnActivatedTodayQuests;
	
	bool CheckTodayQuestsStatus();
	
	void CompleteQuest(FCG_QuestState& InQuest);
	
	bool AreQuestsLoaded() const;
	
	void SetNumQuestsPerDay(const int32 InNumberQuests);
	
	UFUNCTION(BlueprintCallable)
	void UpdateObjective(const FGameplayTag InObjectiveTag);

	UFUNCTION(BlueprintCallable)
	const TArray<FCG_QuestState>& GetTodayQuests() const;
	
	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetTodayQuestsTags();
	
private:
	TArray<TObjectPtr<UCG_QuestData>> AllQuests;
	TArray<FCG_QuestState> TodayQuests;
	bool bQuestLoaded = false;
	int32 NumQuestsToRoll = 2;
	TMap<const FGameplayTag, FObjectiveHandle> ObjectivesHandles;
	
	void ActivateQuest(const UCG_QuestData* InQuest);
	void RollQuests();
	void CacheLoadedQuests();
};
