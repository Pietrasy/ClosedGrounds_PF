// Pietras


#include "Subsystems/DayManager/CG_DayManager.h"

#include "Actors/Spawners/CG_SpawnerBase.h"
#include "Algo/RandomShuffle.h"
#include "ClosedGrounds/ClosedGrounds.h"
#include "Engine/AssetManager.h"
#include "Game/CG_GameInstance.h"
#include "Game/CG_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/QuestManager/CG_QuestManager.h"
#include "Subsystems/SaveManager/CG_SaveManager.h"

class ACG_GameMode;

void UCG_DayManager::InitializeDay()
{
	DetermineCloudForDay();
	SpawnEnemies();
	SpreadFog();
	SelectSpawnPoint();
	StartDay();
	
	OnInitializeDay.ExecuteIfBound();
}

void UCG_DayManager::StartDay()
{
	if (IsValid(TodaySpawner))
	{
		SpawnHouse(TodaySpawner);
	}
	
	SpawnQuestObjects();
	
	UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCG_SaveManager>()->SaveGame();
	
	UE_LOG(LogDayManager, Log, TEXT("[%hs] - Good Morning!"), __FUNCTION__);
	OnDayStarted.Broadcast();
}

void UCG_DayManager::DetermineCloudForDay()
{
	ACG_GameMode* GameMode = Cast<ACG_GameMode>(GetWorld()->GetAuthGameMode());
	CloudsMaterials = GameMode->CloudsMaterials;
	
	if (!CloudsMaterials.IsEmpty())
	{
		Algo::RandomShuffle(CloudsMaterials);

		for (int i = 0; i <= CloudsMaterials.Num(); i++)
		{
			UMaterialInterface* Clouds = CloudsMaterials[i];
		
			if (PreviousClouds != Clouds)
			{
				TodayClouds = Clouds;
				PreviousClouds = TodayClouds;
				UE_LOG(LogDayManager, Log, TEXT("[%hs] - Selected clouds!"), __FUNCTION__);
				return;
			}
		}
	}
}

void UCG_DayManager::SelectSpawnPoint()
{
	TArray<AActor*> HouseSpawnerActors;
	const FName ClassTag = "ESpawnerType::House";
	
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACG_SpawnerBase::StaticClass(), ClassTag, HouseSpawners);
	
	if (!HouseSpawners.IsEmpty())
	{
		Algo::RandomShuffle(HouseSpawners);
		
		for (int i = 0; i <= HouseSpawners.Num(); i++)
		{
			ACG_SpawnerBase* Spawner = Cast<ACG_SpawnerBase>(HouseSpawners[i]);
		
			if (PreviousSpawner != Spawner)
			{
				TodaySpawner = Spawner;
				PreviousSpawner = TodaySpawner;
				UE_LOG(LogDayManager, Log, TEXT("[%hs] - Selected spawn point!"), __FUNCTION__);
				return;
			}
		}
	}
	else
	{
		UE_LOG(LogDayManager, Log, TEXT("[%hs] - Player spawn point doesn't exist!"), __FUNCTION__);
	}
}

void UCG_DayManager::SpawnQuestObjects()
{
	TArray<AActor*> FoundQuestObjectSpawners;
	const FName ClassTag = "ESpawnerType::QuestObject";
	
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACG_SpawnerBase::StaticClass(), ClassTag, FoundQuestObjectSpawners);
	
	const UCG_QuestManager* QuestManager = GetGameInstance()->GetSubsystem<UCG_QuestManager>();
	
	if (!FoundQuestObjectSpawners.IsEmpty())
	{
		for (int SpawnerIndex = 0; SpawnerIndex < FoundQuestObjectSpawners.Num(); ++SpawnerIndex)
		{
			if (FoundQuestObjectSpawners.IsValidIndex(SpawnerIndex)) 
			{
				continue;
			}
			
			QuestManager->ActiveQuests;
		}
	}
	UE_LOG(LogDayManager, Log, TEXT("[%hs] - Spawned quest objects!"), __FUNCTION__);
}

void UCG_DayManager::SpawnEnemies()
{
	TArray<AActor*> FoundEnemySpawners;
	const FName ClassTag = "ESpawnerType::Enemy";
	
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACG_SpawnerBase::StaticClass(), ClassTag, FoundEnemySpawners);

	const UCG_QuestManager* QuestManager = GetGameInstance()->GetSubsystem<UCG_QuestManager>();

	if (!FoundEnemySpawners.IsEmpty())
	{
		for (AActor* Actor : FoundEnemySpawners)
		{
			ACG_SpawnerBase* Spawner = Cast<ACG_SpawnerBase>(Actor);
			if (!IsValid(Spawner)) continue;
			
			if (QuestManager->UsedRegions.Contains(Spawner->SpawnerRegion))
			{
				Spawner->Spawn();
			}
		}
	}
	UE_LOG(LogDayManager, Log,TEXT("[%hs] - Spawned enemies!"), __FUNCTION__);
}

void UCG_DayManager::SpreadFog()
{
	TArray<AActor*> FoundFogSpawners;
	const FName ClassTag = "ESpawnerType::Fog";
	
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACG_SpawnerBase::StaticClass(), ClassTag, FoundFogSpawners);
	
	const UCG_QuestManager* QuestManager = GetGameInstance()->GetSubsystem<UCG_QuestManager>();
	
	if (!FoundFogSpawners.IsEmpty())
	{
		for (int i = 0; i < FoundFogSpawners.Num(); ++i)
		{
			ACG_SpawnerBase* Spawner = Cast<ACG_SpawnerBase>(FoundFogSpawners[i]);

			for (int j = 0; j < QuestManager->UsedRegions.Num(); ++j)
			{
				if (QuestManager->UsedRegions.Contains(Spawner->SpawnerRegion))
				{
					Spawner->Spawn();
				}
			}
		}
	}
	UE_LOG(LogDayManager, Log, TEXT("[%hs] - Spread fogs!"), __FUNCTION__);
}

void UCG_DayManager::EndDay()
{
	UE_LOG(LogDayManager, Log, TEXT("Ending Day"));
	InitializeDay();
	UCG_GameInstance* GI = Cast<UCG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->GetSubsystem<UCG_QuestManager>()->SetQuestObject();
	OnDayEnded.Broadcast();
}

FCG_DaySaveData UCG_DayManager::GetDaySaveData() const
{
	FCG_DaySaveData Data;
	Data.CurrentDay = CurrentDay;
	
	UE_LOG(LogDayManager, Log, TEXT("Save Day Manager"));
	return Data;
}

void UCG_DayManager::LoadDaySaveData(const FCG_DaySaveData& Data)
{
	CurrentDay = Data.CurrentDay;
}

ACG_SpawnerBase* UCG_DayManager::FindSpawnerClosestToPlayer()
{
	AActor* Closest = nullptr;
	// ???
	float BestDisSq = FLT_MAX;
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector Location = PlayerPawn->GetActorLocation();

	for (AActor* Spawner : HouseSpawners)
	{
		if (!IsValid(Spawner)) continue;
		
		float DistSq = FVector::DistSquared(Location, Spawner->GetActorLocation());
		if (DistSq < BestDisSq)
		{
			BestDisSq = DistSq;
			Closest = Spawner;
		}
	}
	
	return Cast<ACG_SpawnerBase>(Closest);
}

void UCG_DayManager::SpawnHouse(ACG_SpawnerBase* HouseSpawner)
{
	if (!IsValid(HouseSpawner))
	{
		UE_LOG(LogDayManager, Log, TEXT("[%hs] - HouseSpawner isn't valid!"), __FUNCTION__);
		return;
	}
	
	HouseSpawner->Spawn();
}
