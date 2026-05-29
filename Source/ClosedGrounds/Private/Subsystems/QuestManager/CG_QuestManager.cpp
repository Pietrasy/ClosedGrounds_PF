// Pietras


#include "Subsystems/QuestManager/CG_QuestManager.h"

#include "GameplayTagContainer.h"
#include "ClosedGrounds/ClosedGrounds.h"
#include "Engine/AssetManager.h"
#include "Game/CG_GameInstance.h"
#include "Subsystems/QuestManager/Data/CG_QuestData.h"
#include "Subsystems/QuestManager/Data/FCG_QuestState.h"
#include "Subsystems/QuestManager/Data/FCG_ObjectiveState.h"

void UCG_QuestManager::InitializeQuestSystem()
{
	const UCG_GameInstance* GameInstance = Cast<UCG_GameInstance>(GetGameInstance());
	checkf(GameInstance, TEXT("[%hs] - GameInstance isn't UCG_GameInstance!"), __FUNCTION__);
	
	if (!UAssetManager::IsInitialized())
	{
		UE_LOG(LogQuestManager, Error, TEXT("[%hs] - AssetManager isn't initialized yet!"), __FUNCTION__);
		return;
	}
	
	TArray<FPrimaryAssetId> QuestPrimaryAssets;
	UAssetManager::Get().GetPrimaryAssetIdList(FPrimaryAssetType("Quest"), QuestPrimaryAssets);
	
	if (QuestPrimaryAssets.IsEmpty())
	{
		UE_LOG(LogQuestManager, Warning, TEXT("[%hs] - There aren't quests to load from Asset Manager!"), __FUNCTION__);
		return;
	}
	
	UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Loading %d quest assets..."), __FUNCTION__, QuestPrimaryAssets.Num());

	const FStreamableDelegate OnAsyncAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &UCG_QuestManager::CacheLoadedQuests);
	UAssetManager::Get().LoadPrimaryAssets(QuestPrimaryAssets, TArray<FName>(), OnAsyncAssetsLoadedDelegate);
}

void UCG_QuestManager::CacheLoadedQuests()
{
	UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Quests have been loaded!"), __FUNCTION__);
	
	TArray<UObject*> ObjectList;
	UAssetManager::Get().GetPrimaryAssetObjectList(FPrimaryAssetType("Quest"), ObjectList);

	for (UObject* Element : ObjectList)
	{
		AllQuests.Add(Cast<UCG_QuestData>(Element));
	}
	
	bQuestLoaded = true;
	RollQuests();
}

void UCG_QuestManager::RollQuests()
{
	NumQuestsToRoll = FMath::Min(NumQuestsToRoll, AllQuests.Num());
	
	TArray<TObjectPtr<UCG_QuestData>> CachedAllQuests = AllQuests;

	for (int32 Index = 0; Index < CachedAllQuests.Num(); ++Index)
	{
		const int32 SwapIndex = FMath::RandRange(Index, CachedAllQuests.Num() - 1);
		CachedAllQuests.Swap(Index, SwapIndex);
	}

	for (int32 Index = 0; Index < NumQuestsToRoll; ++Index)
	{
		UCG_QuestData* SelectedQuest = CachedAllQuests[Index];
		
		if (!IsValid(SelectedQuest))
		{
			UE_LOG(LogQuestManager, Error, TEXT("[%hs] - Invalid selected quest!"), __FUNCTION__);
			continue;
		}

		ActivateQuest(SelectedQuest);
	}
	OnActivatedTodayQuests.ExecuteIfBound();
}

void UCG_QuestManager::ActivateQuest(const UCG_QuestData* InQuest)
{
	FCG_QuestState NewQuest;
	
	NewQuest.QuestName = InQuest->QuestName;
	NewQuest.QuestObjectives = InQuest->QuestObjectives;
	NewQuest.Region = InQuest->QuestRegion;
	NewQuest.QuestTag = InQuest->QuestTag;
	
	FObjectiveHandle ObjectiveHandle;
	ObjectiveHandle.QuestIndex = TodayQuests.Add(NewQuest);
	
	for (int Index = 0; Index < NewQuest.QuestObjectives.Num(); ++Index)
	{
		ObjectiveHandle.ObjectiveIndex = Index;
		ObjectivesHandles.Add(NewQuest.GetQuestObjective(Index)->ObjectiveTag, ObjectiveHandle);
	}

	UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Activated quest: %s"), __FUNCTION__, *InQuest->QuestName.ToString());
}

void UCG_QuestManager::CompleteQuest(FCG_QuestState& InQuest)
{
	InQuest.IsCompleted();
	UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Quest %s has been completed!"), __FUNCTION__, *InQuest.QuestName.ToString());
	
	CheckTodayQuestsStatus();
}

void UCG_QuestManager::UpdateObjective(const FGameplayTag InObjectiveTag)
{
	if (!InObjectiveTag.IsValid())
	{
		UE_LOG(LogQuestManager, Error, TEXT("[%hs] - Tag isn't valid!"), __FUNCTION__);
		return;
	}
	
	const FObjectiveHandle* ObjectiveHandle = ObjectivesHandles.Find(InObjectiveTag);

	if (!ObjectiveHandle)
	{
		UE_VLOG_UELOG(this, LogQuestManager, Error, TEXT("[%hs] - Invalid ObjectiveHandle!"), __FUNCTION__);
		return;
	}
	
	const int32 QuestIndex = ObjectiveHandle->QuestIndex;
	const int32 ObjectiveIndex = ObjectiveHandle->ObjectiveIndex;
	
	FCG_ObjectiveState* Objective = TodayQuests[QuestIndex].GetQuestObjective(ObjectiveIndex);
	Objective->IncrementProgress();
	UE_LOG(LogQuestManager, Log, TEXT("Objective progress [%s]: %d/%d"), *InObjectiveTag.ToString(), Objective->CurrentProgress, Objective->RequiredProgress);
	
	if (TodayQuests[QuestIndex].IsCompleted())
	{
		CompleteQuest(TodayQuests[QuestIndex]);
	}
}

bool UCG_QuestManager::CheckTodayQuestsStatus()
{
	bool bAreQuestsFinished = true;
	
	for (const FCG_QuestState& Quest : TodayQuests)
	{
		if (!Quest.bIsCompleted)
		{
			bAreQuestsFinished = false;
		}
	}

	if (bAreQuestsFinished)
	{
		OnFinishedTodayQuests.Broadcast();
		UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Finished all quests today!"), __FUNCTION__);
	}
	
	return bAreQuestsFinished;
}

const TArray<FCG_QuestState>& UCG_QuestManager::GetTodayQuests() const
{
	if (TodayQuests.IsEmpty())
	{
		UE_VLOG_UELOG(this, LogQuestManager, Warning, TEXT("[%hs] - TodayQuests is empty!"), __FUNCTION__);
	}
	
	return TodayQuests;
}

FGameplayTagContainer UCG_QuestManager::GetTodayQuestsTags()
{
	if (TodayQuests.IsEmpty())
	{
		UE_VLOG_UELOG(this, LogQuestManager, Warning, TEXT("[%hs] - TodayQuests is empty!"), __FUNCTION__);
	}
	
	FGameplayTagContainer GameplayTags;

	for (FCG_QuestState& Quest : TodayQuests)
	{
		GameplayTags.AddTag(Quest.QuestTag);
	}
	
	return GameplayTags;
}

bool UCG_QuestManager::AreQuestsLoaded() const
{
	return bQuestLoaded;
}

void UCG_QuestManager::SetNumQuestsPerDay(const int32 InNumberQuests) 
{
	NumQuestsToRoll = InNumberQuests;
}