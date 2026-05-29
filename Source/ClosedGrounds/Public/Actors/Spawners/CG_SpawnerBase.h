// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/ECG_Region.h"
#include "Data/ECG_SpawnerType.h"
#include "GameFramework/Actor.h"
#include "CG_SpawnerBase.generated.h"

class UTextRenderComponent;
class UBillboardComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawn);

UCLASS(Abstract)
class CLOSEDGROUNDS_API ACG_SpawnerBase : public AActor
{
	GENERATED_BODY()

public:
	ACG_SpawnerBase();
	
	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSpawn OnSpawn;
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	ERegion SpawnerRegion = ERegion::Null;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ESpawnerType SpawnerType;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsAlwaysSpawn = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool SpawnedOnEndDay = false;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag SpawnerTag = FGameplayTag::EmptyTag;
	
	UFUNCTION(BlueprintNativeEvent)
	void Spawn();
	
protected:
	UFUNCTION(BlueprintCallable)
	void OnInteracted();
	
private:
	UPROPERTY()
	TObjectPtr<UBillboardComponent> BillboardComponent;
	
	
};
