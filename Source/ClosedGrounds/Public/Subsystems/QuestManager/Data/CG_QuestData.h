// Pietras

#pragma once

#include "CoreMinimal.h"
#include "FCG_ObjectiveState.h"
#include "Data/ECG_Region.h"
#include "Engine/DataAsset.h"
#include "CG_QuestData.generated.h"

/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_QuestData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName QuestName;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FCG_ObjectiveState> QuestObjectives;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ERegion QuestRegion = ERegion::Null;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag QuestTag = FGameplayTag::EmptyTag;
};
