// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Data/ECG_Region.h"
#include "Data/FCG_ActiveQuest.h"
#include "Subsystems/SaveManager/Data/FCG_QuestSaveData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CG_QuestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedAllQuestsDynamic);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadedAllQuests);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestSystemReady);

class UCG_QuestData;
/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_QuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void InitializeQuestSystem();
	
	UPROPERTY(BlueprintAssignable)
	FOnQuestSystemReady OnQuestSystemReady;
	
	UPROPERTY(BlueprintAssignable)
	FOnFinishedAllQuestsDynamic OnFinishedAllQuests;
	UPROPERTY(BlueprintAssignable)
	FOnFinishedAllQuestsDynamic FOnLoadedAllQuests;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TObjectPtr<UCG_QuestData>> RemainingQuests;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FCG_ActiveQuest> ActiveQuests;
	
	TMap<FGameplayTag, TArray<FCG_QuestObjective*>> ObjectivesByTag;
	
	UFUNCTION(BlueprintCallable)
	void DrawQuests();
	
	void ActivateQuests(UCG_QuestData* QuestData);
	void RegisterObjectives(FCG_ActiveQuest& Quest);
	void UnregisterObjectives(FCG_ActiveQuest& Quest);
	
	UFUNCTION(BlueprintCallable)
	void NotifyObjective(FGameplayTag QuestTag);
	
	TSet<ERegion> UsedRegions;
	
	void ClearActiveQuests();
	
	void CompleteQuest(FCG_ActiveQuest& Quest);
	
	FCG_QuestSaveData GetQuestSaveData();
	void LoadQuestSaveData(FCG_QuestSaveData& SaveData);
	
	void SetQuestObject();
	
	void CompleteDay();
	
	TSet<FGameplayTag> GetActiveObjectiveTags();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FCG_ActiveQuest> Quests;
	
	void OnAssetsLoaded();
	
	TArray<FPrimaryAssetId> QuestAssetIds;
	
	UPROPERTY()
	TArray<UCG_QuestData*> LoadedQuests;
	
	void AddQuest(UCG_QuestData* Quest);
};
