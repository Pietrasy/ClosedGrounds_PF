// Pietras


#include "Subsystems/QuestManager/CG_QuestManager.h"

#include "GameplayTagContainer.h"
#include "Actors/Spawners/CG_SpawnerBase.h"
#include "Algo/RandomShuffle.h"
#include "ClosedGrounds/ClosedGrounds.h"
#include "Engine/AssetManager.h"
#include "Game/CG_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/CG_AssetManager.h"
#include "Subsystems/DayManager/CG_DayManager.h"
#include "Subsystems/QuestManager/Data/CG_QuestData.h"
#include "Subsystems/QuestManager/Data/FCG_ActiveQuest.h"
#include "Subsystems/QuestManager/Data/FCG_QuestObjective.h"

void UCG_QuestManager::InitializeQuestSystem()
{
	if (!UAssetManager::IsInitialized())
	{
		UE_LOG(LogTemp, Warning, TEXT("AssetManager not initialized yet!"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem: InitializeQuestSystem START"));

	UAssetManager& Manager = UCG_AssetManager::Get();
	
	TArray<FPrimaryAssetId> DebugIds;
	Manager.GetPrimaryAssetIdList(UCG_AssetManager::QuestType, DebugIds);

	UE_LOG(LogTemp, Warning, TEXT("Quest Assets Found: %d"), DebugIds.Num());

	for (auto& Id : DebugIds)
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset: %s"), *Id.ToString());
	}

	// 🔹 1. Pobierz wszystkie Quest assety
	Manager.GetPrimaryAssetIdList(UCG_AssetManager::QuestType, QuestAssetIds);

	if (QuestAssetIds.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No quest assets found!"));
		OnQuestSystemReady.Broadcast();
		return;
	}

	// 🔹 2. Async load
	FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(
		this,
		&UCG_QuestManager::OnAssetsLoaded
	);

	Manager.LoadPrimaryAssets(QuestAssetIds, TArray<FName>(), Delegate);

	UE_LOG(LogTemp, Warning, TEXT("Loading %d quest assets..."), QuestAssetIds.Num());
}

void UCG_QuestManager::DrawQuests()
{
	const UCG_GameInstance* GameInstance = Cast<UCG_GameInstance>(GetGameInstance());
    if (!GameInstance)
    {
        return;
    }

    // Get all quest IDs from Asset Manager
    TArray<FPrimaryAssetId> QuestIDs;
    UAssetManager::Get().GetPrimaryAssetIdList(FPrimaryAssetType("Quest"), QuestIDs);

    if (QuestIDs.IsEmpty())
    {
        UE_LOG(LogQuestManager, Error, TEXT("[%hs] - There aren't quests to load!"), __FUNCTION__);
        return;
    }

    UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Quests found: %d"), __FUNCTION__, QuestIDs.Num());

    // Shuffle
    TArray<FPrimaryAssetId> ShuffledQuestArray = QuestIDs;
    Algo::RandomShuffle(ShuffledQuestArray);

    UAssetManager& AssetManager = UAssetManager::Get();

    RemainingQuests.Reset();

    // Ile questów chcemy wylosować (docelowo do settingsów)
    const int32 NumQuestsToDraw = FMath::Min(2, ShuffledQuestArray.Num());

    // Weak pointer dla bezpieczeństwa (async)
    TWeakObjectPtr<UCG_QuestManager> WeakThis(this);

	TSharedRef<int32> PendingLoads = MakeShared<int32>(NumQuestsToDraw);

    for (int32 QuestIndex = 0; QuestIndex < NumQuestsToDraw; ++QuestIndex)
    {
        const FPrimaryAssetId QuestID = ShuffledQuestArray[QuestIndex];

    	UCG_QuestData* Existing = Cast<UCG_QuestData>(
	AssetManager.GetPrimaryAssetObject(QuestID));
    	
    	if (Existing)
    	{
    		UE_LOG(LogQuestManager, Warning, TEXT("Already loaded: %s"), *QuestID.ToString());

    		RemainingQuests.Add(Existing);

    		(*PendingLoads)--;

    		if (*PendingLoads == 0)
    		{
    			UE_LOG(LogQuestManager, Log, TEXT("All quests loaded (sync path). Activating... (%d)"), RemainingQuests.Num());

    			for (UCG_QuestData* LoadedQuest : RemainingQuests)
    			{
    				ActivateQuests(LoadedQuest);
    			}

    			FOnLoadedAllQuests.Broadcast();
    		}

    		continue;
    	}
    	
    	FSoftObjectPath Path = AssetManager.GetPrimaryAssetPath(QuestID);

    	UE_LOG(LogQuestManager, Warning, TEXT("Path: %s"), *Path.ToString());
    	
    	UE_LOG(LogQuestManager, Warning, TEXT("Requesting load for: %s"), *QuestID.ToString());
        AssetManager.LoadPrimaryAsset(
            QuestID,
            {},
            FStreamableDelegate::CreateLambda([WeakThis, QuestID, PendingLoads]()
            {
            	UE_LOG(LogQuestManager, Warning, TEXT("Lambda fired for QuestID: %s"), *QuestID.ToString());
                if (!WeakThis.IsValid())
                {
                	UE_LOG(LogQuestManager, Error, TEXT("QuestManager destroyed before load finished"));
                    return;
                }

                UCG_QuestManager* Self = WeakThis.Get();
                UAssetManager& AM = UAssetManager::Get();

                UCG_QuestData* QuestData = Cast<UCG_QuestData>(
                    AM.GetPrimaryAssetObject(QuestID)
                );
            	
                if (IsValid(QuestData))
                {
                    Self->RemainingQuests.Add(QuestData);

                    UE_LOG(LogQuestManager, Log, TEXT("Loaded quest: %s"), *QuestData->QuestName.ToString());
                }
                else
                {
                    UE_LOG(LogQuestManager, Warning, TEXT("Failed to load quest for ID: %s"), *QuestID.ToString());
                }
            	
            	(*PendingLoads)--;
            	

if (*PendingLoads == 0)
{
	UE_LOG(LogQuestManager, Log, TEXT("All quests loaded. Activating... (%d)"), Self->RemainingQuests.Num());

	for (UCG_QuestData* LoadedQuest : Self->RemainingQuests)
	{
		Self->ActivateQuests(LoadedQuest);
	}

	Self->FOnLoadedAllQuests.Broadcast();
}
            })
        );
    }
}

void UCG_QuestManager::ActivateQuests(UCG_QuestData* QuestData)
{
	if (!IsValid(QuestData)) return;

	FCG_ActiveQuest NewQuest;

	// ✅ poprawne przypisania
	NewQuest.QuestData = QuestData;
	NewQuest.QuestID = QuestData->GetPrimaryAssetId();
	NewQuest.Objectives = QuestData->QuestObjectives;

	// (opcjonalnie)
	UsedRegions.Add(QuestData->QuestRegion);

	// ⚠️ dodajemy do tablicy
	int32 QuestIndex = ActiveQuests.Add(NewQuest);
	FCG_ActiveQuest& Quest = ActiveQuests[QuestIndex];

	// ✅ KLUCZOWE
	RegisterObjectives(Quest);

	UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Activated quest: %s"), __FUNCTION__, *QuestData->QuestName.ToString());
}

void UCG_QuestManager::RegisterObjectives(FCG_ActiveQuest& Quest)
{
	for (FCG_QuestObjective& Objective : Quest.Objectives)
	{
		// 🔥 NAJWAŻNIEJSZE
		Objective.OwningQuest = &Quest;
		
		UE_LOG(LogQuestManager, Warning,
	TEXT("REGISTER Objective Tag: %s"),
	*Objective.ObjectiveTag.ToString());

		ObjectivesByTag
			.FindOrAdd(Objective.ObjectiveTag)
			.Add(&Objective);
	}
}

void UCG_QuestManager::UnregisterObjectives(FCG_ActiveQuest& Quest)
{
	for (FCG_QuestObjective& Objective : Quest.Objectives)
	{
		if (TArray<FCG_QuestObjective*>* Array = ObjectivesByTag.Find(Objective.ObjectiveTag))
		{
			Array->Remove(&Objective);

			if (Array->IsEmpty())
			{
				ObjectivesByTag.Remove(Objective.ObjectiveTag);
			}
		}
	}
}

void UCG_QuestManager::NotifyObjective(FGameplayTag QuestTag)
{
	if (!QuestTag.IsValid())
	{
		UE_LOG(LogQuestManager, Warning, TEXT("NotifyObjective: Invalid tag"));
		return;
	}
	
	UE_LOG(LogQuestManager, Warning,
	TEXT("NOTIFY Tag: %s"),
	*QuestTag.ToString());

	TArray<FCG_QuestObjective*>* Objectives = ObjectivesByTag.Find(QuestTag);
	if (!Objectives)
	{
		UE_LOG(LogQuestManager, Verbose, TEXT("No objectives found for tag: %s"), *QuestTag.ToString());
		return;
	}

	TArray<FCG_ActiveQuest*> QuestsToComplete;

	for (FCG_QuestObjective* Objective : *Objectives)
	{
		if (!Objective) continue;

		// 🔒 safety – jeśli quest już nie istnieje
		if (!Objective->OwningQuest) continue;

		// ⛔ już ukończony → skip
		if (Objective->IsCompleted()) continue;

		// ✅ increment (z clampem)
		Objective->CurrentAmount = FMath::Min(
			Objective->CurrentAmount + 1,
			Objective->RequiredAmount
		);

		UE_LOG(LogQuestManager, Log,
			TEXT("Objective progress [%s]: %d/%d"),
			*QuestTag.ToString(),
			Objective->CurrentAmount,
			Objective->RequiredAmount
		);

		// 🎯 jeśli objective właśnie się ukończył
		if (Objective->IsCompleted())
		{
			FCG_ActiveQuest* OwningQuest = Objective->OwningQuest;

			if (OwningQuest->IsCompleted())
			{
				QuestsToComplete.AddUnique(OwningQuest);
			}
		}
	}

	// 🔥 finalize questy (po pętli → unikamy modyfikacji w trakcie iteracji)
	for (FCG_ActiveQuest* Quest : QuestsToComplete)
	{
		if (!Quest) continue;

		UE_LOG(LogQuestManager, Log,
			TEXT("Quest ready to complete: %s"),
			*Quest->QuestData->QuestName.ToString()
		);

		CompleteQuest(*Quest);
	}
}

void UCG_QuestManager::ClearActiveQuests()
{
	UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Clear active quests!"), __FUNCTION__);
}

void UCG_QuestManager::CompleteQuest(FCG_ActiveQuest& Quest)
{
	UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Quest completed!"), __FUNCTION__);
	
	for (int i = 0; i < ActiveQuests.Num(); i++)
	{
		if (ActiveQuests[i].IsCompleted())
		{
			UnregisterObjectives(ActiveQuests[i]); // 🔥 NOWE

			ActiveQuests.RemoveAt(i);
			i--;
			UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Quest removed!"), __FUNCTION__);

			if (ActiveQuests.IsEmpty())
			{
				CompleteDay();
			}
		}
	}
}

FCG_QuestSaveData UCG_QuestManager::GetQuestSaveData()
{
	FCG_QuestSaveData Data;
	
	for (const FCG_ActiveQuest& ActiveQuest : ActiveQuests)
	{
		Data.ActiveQuests.Add(ActiveQuest.QuestData->GetPrimaryAssetId());
	}
	
	for (const UCG_QuestData* RemainingQuest : RemainingQuests)
	{
		if (Data.RemainingQuests.IsEmpty())
		{
			continue;
		}
		Data.RemainingQuests.Add(RemainingQuest->GetPrimaryAssetId());
	}
	
	UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Save quests!"), __FUNCTION__);
	return Data;
}

void UCG_QuestManager::LoadQuestSaveData(FCG_QuestSaveData& SaveData)
{
	ActiveQuests.Empty();
	
	UAssetManager& AssetManager = UAssetManager::Get();
	
	auto IDs = UAssetManager::Get().GetPrimaryAssetIdList(FPrimaryAssetType("Quest"),SaveData.ActiveQuests);
	UE_LOG(LogQuestManager, Warning, TEXT("[%hs] - Quest ID from AssetManager: %hhd"), __FUNCTION__, IDs);
	
	for (FPrimaryAssetId QuestID : SaveData.ActiveQuests)
	{
		// Load asset asynchronously
		auto Handle = AssetManager.LoadPrimaryAsset(QuestID);
		
		if (!Handle.IsValid())
		{
			UE_LOG(LogQuestManager, Warning, TEXT("[%hs] - Quest: %s is unable to load!"),__FUNCTION__, *QuestID.ToString());
			continue;
		}
		
		Handle->WaitUntilComplete();
		
		UObject* QuestDataAsset = AssetManager.GetPrimaryAssetObject(QuestID);
		
		if (UCG_QuestData* Quest = Cast<UCG_QuestData>(QuestDataAsset))
		{
			UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Loaded Quest: %s"), __FUNCTION__, *Quest->QuestName.ToString());
			AddQuest(Quest);
		}
	}
	
	UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Load quests!"), __FUNCTION__);

}

void UCG_QuestManager::SetQuestObject()
{
	TArray<AActor*> FoundSpawners;
	const FName ClassTag = "ESpawnerType::QuestObject";

	UGameplayStatics::GetAllActorsOfClassWithTag(
		GetWorld(),
		ACG_SpawnerBase::StaticClass(),
		ClassTag,
		FoundSpawners
	);

	if (FoundSpawners.IsEmpty())
	{
		UE_LOG(LogQuestManager, Log, TEXT("No quest spawners found"));
		return;
	}

	const TSet<FGameplayTag> ActiveTags = GetActiveObjectiveTags();

	for (AActor* Actor : FoundSpawners)
	{
		ACG_SpawnerBase* Spawner = Cast<ACG_SpawnerBase>(Actor);
		if (!Spawner) continue;

		// 🔥 KLUCZOWY WARUNEK
		if (ActiveTags.Contains(Spawner->SpawnerTag))
		{
			Spawner->Spawn();

			UE_LOG(LogQuestManager, Log,
				TEXT("Spawned spawner with tag: %s"),
				*Spawner->SpawnerTag.ToString());
		}
	}
		/*UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Spawn quest object spawners!"), __FUNCTION__);
	}
	else
	{
		UE_LOG(LogQuestManager, Log, TEXT("[%hs] - There aren't quest object spawner to spawn!"), __FUNCTION__);
	}*/
}

void UCG_QuestManager::CompleteDay()
{
	OnFinishedAllQuests.Broadcast();
	UCG_DayManager* DayManager = GetGameInstance()->GetSubsystem<UCG_DayManager>();
	ACG_SpawnerBase* HouseSpawner = DayManager->FindSpawnerClosestToPlayer();
	HouseSpawner->SpawnedOnEndDay = true;
	DayManager->SpawnHouse(HouseSpawner);
				
	UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Finished all quests today!"), __FUNCTION__);
}

TSet<FGameplayTag> UCG_QuestManager::GetActiveObjectiveTags()
{
	TSet<FGameplayTag> Tags;

	for (const FCG_ActiveQuest& Quest : ActiveQuests)
	{
		for (const FCG_QuestObjective& Obj : Quest.Objectives)
		{
			if (!Obj.IsCompleted())
			{
				Tags.Add(Obj.ObjectiveTag);
			}
		}
	}

	return Tags;
}

void UCG_QuestManager::OnAssetsLoaded()
{
	UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem: Assets Loaded"));

	UAssetManager& Manager = UCG_AssetManager::Get();

	LoadedQuests.Empty();

	for (const FPrimaryAssetId& Id : QuestAssetIds)
	{
		UObject* Obj = Manager.GetPrimaryAssetObject(Id);

		if (UCG_QuestData* Quest = Cast<UCG_QuestData>(Obj))
		{
			LoadedQuests.Add(Quest);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Loaded Quests: %d"), LoadedQuests.Num());

	// 🔹 TU robisz swoją logikę
	// BuildObjectivesIndex();
	// SpawnQuestObjects();
	
	UCG_GameInstance* InGameInstance = Cast<UCG_GameInstance>(GetGameInstance());

	DrawQuests();
	InGameInstance->GetSubsystem<UCG_DayManager>()->InitializeDay();
	SetQuestObject();
	
	if (InGameInstance)
	{
		InGameInstance->OnQuestSystemReadyDynamic.Broadcast();
	}

	OnQuestSystemReady.Broadcast();
}

void UCG_QuestManager::AddQuest(UCG_QuestData* Quest)
{
	FCG_ActiveQuest NewQuest;
	NewQuest.QuestID = Quest->GetPrimaryAssetId();
	NewQuest.QuestData = Quest;
	NewQuest.Objectives = Quest->QuestObjectives;
	ActiveQuests.Add(NewQuest);
	UsedRegions.Add(Quest->QuestRegion);
}
