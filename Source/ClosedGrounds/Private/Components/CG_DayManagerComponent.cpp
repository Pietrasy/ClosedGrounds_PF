// Pietras


#include "Components/CG_DayManagerComponent.h"

#include "Actors/GameplayActors/CG_GameplayActor.h"
#include "ClosedGrounds/ClosedGrounds.h"
#include "Game/CG_GameMode.h"
#include "Subsystems/QuestManager/CG_QuestManager.h"

void UCG_DayManagerComponent::RegisterSpawner(ACG_GameplayActor* InGameplayActor)
{
	switch (InGameplayActor->GetType())
	{
	case EGameplayActorType::EnemySpawner:
		EnemySpawners.Add(InGameplayActor);
		break;
	case EGameplayActorType::HouseActor:
		HouseActors.Add(InGameplayActor);
		break;
	case EGameplayActorType::QuestActor:
		QuestActors.Add(InGameplayActor);
		default:
		break;
	}
}

void UCG_DayManagerComponent::InitializeDayManager()
{
	CGGameMode = GetOwner<ACG_GameMode>();
	checkf(CGGameMode, TEXT("CGGameMode isn't an ACG_GameMode!"));
	
	UCG_QuestManager* QuestManager = CGGameMode->GetQuestManager();

	if (!QuestManager)
	{
		UE_LOG(LogGame, Error, TEXT("[%hs] - Invalid QuestManager!"), __FUNCTION__);
		return;
	}
	
	if (!QuestManager->AreQuestsLoaded())
	{
		QuestManager->OnActivatedTodayQuests.BindUFunction(this, TEXT("InitializeDay"));
		return;
	}
	
	InitializeDay();
}

void UCG_DayManagerComponent::InitializeDay()
{
	DetermineClouds();
	SpawnEnemies();
	SelectHouseActor();
	ActivateQuestActors();
	
	OnInitializedDay.ExecuteIfBound();
	UE_VLOG_UELOG(this, LogDayManager, Log, TEXT("[%hs] - Initialized day."), __FUNCTION__);
}

void UCG_DayManagerComponent::DetermineClouds()
{
	if (CloudMaterials.IsEmpty())
	{
		UE_VLOG_UELOG(this, LogDayManager, Warning, TEXT("[%hs] - CloudsMaterials is empty!"), __FUNCTION__);
		return;
	}
	
	TodayCloudsIndex = FMath::RandRange(0, CloudMaterials.Num()-1);
	
	if (TodayCloudsIndex == PreviousCloudsIndex)
	{
		if (!CloudMaterials.IsValidIndex(++TodayCloudsIndex))
		{
			TodayCloudsIndex = 0;
		}
	}
	
	TodayVolumetricCloud = CloudMaterials[TodayCloudsIndex].LoadSynchronous();
	PreviousCloudsIndex = TodayCloudsIndex;
	
	UE_VLOG_UELOG(this, LogDayManager, Log, TEXT("[%hs] - Determined clouds."), __FUNCTION__);
}

void UCG_DayManagerComponent::SpawnEnemies()
{
	if (EnemySpawners.IsEmpty())
	{
		UE_VLOG_UELOG(this, LogDayManager, Warning, TEXT("[%hs] - EnemySpawners is empty!"), __FUNCTION__);
		return;
	}
	
	for (auto Spawner : EnemySpawners)
	{
		Spawner.Get()->Activate();
	}
	
	UE_VLOG_UELOG(this, LogDayManager, Log, TEXT("[%hs] - Spawned enemies."), __FUNCTION__);
}

void UCG_DayManagerComponent::SelectHouseActor()
{
		if (HouseActors.IsEmpty())
	{
		UE_VLOG_UELOG(this, LogDayManager, Warning, TEXT("[%hs] - HouseActors is empty!"), __FUNCTION__);
		return;
	}
	
	TodayHouseActorIndex = FMath::RandRange(0, HouseActors.Num() - 1);

	if (TodayHouseActorIndex == PreviousSpawnPointIndex)
	{
		if (!HouseActors.IsValidIndex(++TodayHouseActorIndex))
		{
			TodayHouseActorIndex = 0;
		}
	}
	
	if (!HouseActors[TodayHouseActorIndex].IsValid())
	{
		UE_VLOG_UELOG(this, LogDayManager, Warning, TEXT("[%hs] - House isn't valid!"), __FUNCTION__);
		return;
	} 
	
	UE_VLOG_UELOG(this, LogDayManager, Log, TEXT("[%hs] - Selected House."), __FUNCTION__);
}

void UCG_DayManagerComponent::ActivateQuestActors()
{
	if (QuestActors.IsEmpty())
	{
		UE_VLOG_UELOG(this, LogDayManager, Warning, TEXT("[%hs] - QuestObjectSpawner is empty!"), __FUNCTION__);
		return;
	}
	
	for (auto Spawner : QuestActors)
	{
		Spawner.Get()->Activate();
	}
	
	UE_VLOG_UELOG(this, LogDayManager, Log, TEXT("[%hs] - Spawned quest objects."), __FUNCTION__);
}

UMaterialInstance*  UCG_DayManagerComponent::GetTodayVolumetricCloudMaterial() const
{
	if (!CloudMaterials.IsValidIndex(TodayCloudsIndex))
	{
		UE_LOG(LogGame, Warning, TEXT("[%hs] - CloudsMaterials index isn't valid!"), __FUNCTION__);
		return nullptr;
	}
	
	UMaterialInstance* CloudMaterial = CloudMaterials[TodayCloudsIndex].LoadSynchronous();
	return CloudMaterial;
}

void UCG_DayManagerComponent::SetVolumetricClouds(const TArray<TSoftObjectPtr<UMaterialInstance>>& InClouds)
{
	CloudMaterials.Append(InClouds);
}

void UCG_DayManagerComponent::SetPlayerSpawn()
{
	if (!HouseActors.IsValidIndex(TodayHouseActorIndex))
	{
		UE_LOG(LogDayManager, Error, TEXT("[%hs] - HouseActors index isn't valid!"), __FUNCTION__);
		return;
	}
	
	HouseActors[TodayHouseActorIndex].Get()->Activate();
}
