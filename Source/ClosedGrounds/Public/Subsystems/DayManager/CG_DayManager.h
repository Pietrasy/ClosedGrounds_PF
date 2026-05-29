// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/World.h"
#include "Subsystems/SaveManager/Data/FCG_DaySaveData.h"
#include "CG_DayManager.generated.h"

class ACG_SpawnerBase;
DECLARE_DELEGATE(FOnInitializeDaySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDayStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDayEnded);

class UWorld;

/**
 * 
 */
UCLASS() 
class CLOSEDGROUNDS_API UCG_DayManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// --- Delegates ---
	FOnInitializeDaySignature OnInitializeDay;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDayStarted OnDayStarted;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDayEnded OnDayEnded;
	// --- Delegates ---
	
	UFUNCTION(BLueprintCallable)
	void InitializeDay();
	
	UFUNCTION(BLueprintCallable)
	void StartDay();
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> TodayClouds;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACG_SpawnerBase> TodaySpawner;
	
	UFUNCTION(BlueprintCallable)
	void EndDay();
	
	FCG_DaySaveData GetDaySaveData() const;
	void LoadDaySaveData(const FCG_DaySaveData& Data);
	
	void SpawnHouse(ACG_SpawnerBase* HouseSpawner);
	
	ACG_SpawnerBase* FindSpawnerClosestToPlayer();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UMaterialInstance>> CloudsMaterials;
	
	UPROPERTY()
	TArray<AActor*> HouseSpawners;
	
	UPROPERTY()
	TObjectPtr<UMaterialInterface> PreviousClouds;
	UPROPERTY()
	TObjectPtr<AActor> PreviousSpawner;
	
	void DetermineCloudForDay();
	
	void SelectSpawnPoint();
	
	void SpawnQuestObjects();
	
	void SpawnEnemies();
	
	void SpreadFog();
	
	UPROPERTY()
	int32 CurrentDay = 1;
};
