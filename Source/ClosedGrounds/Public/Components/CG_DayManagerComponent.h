// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CG_DayManagerComponent.generated.h"

DECLARE_DELEGATE(FOnInitializedDay)

class ACG_GameplayActor;
class ACG_GameMode;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CLOSEDGROUNDS_API UCG_DayManagerComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	FOnInitializedDay OnInitializedDay;
	
	void RegisterSpawner(ACG_GameplayActor* InGameplayActor);
	
	void InitializeDayManager();
	
	void SetVolumetricClouds(const TArray<TSoftObjectPtr<UMaterialInstance>>& InClouds);
	
	void SetPlayerSpawn();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstance* GetTodayVolumetricCloudMaterial() const;

private:
	UPROPERTY()
	TObjectPtr<ACG_GameMode> CGGameMode;
	
	TArray<TSoftObjectPtr<UMaterialInstance>> CloudMaterials;
	
	uint32 PreviousCloudsIndex;
	uint32 TodayCloudsIndex;
	
	uint32 PreviousSpawnPointIndex;
	uint32 TodayHouseActorIndex;
	
	TArray<TWeakObjectPtr<ACG_GameplayActor>> EnemySpawners;
	TArray<TWeakObjectPtr<ACG_GameplayActor>> HouseActors;
	TArray<TWeakObjectPtr<ACG_GameplayActor>> QuestActors;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstance> TodayVolumetricCloud;
	
	UFUNCTION(BlueprintCallable)
	void InitializeDay();
	
	void DetermineClouds();
	
	void SpawnEnemies();
	
	void SelectHouseActor();
	
	void ActivateQuestActors();
};
