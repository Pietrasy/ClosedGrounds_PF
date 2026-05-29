// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CG_GameInstance.generated.h"



class UCG_QuestManager;
class UCG_DayManager;

class UCG_QuestData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestSystemReadyDynamic);
/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnQuestSystemReadyDynamic OnQuestSystemReadyDynamic;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TObjectPtr<UCG_QuestData>>  AllQuests;
	
private:
	UPROPERTY()
	TObjectPtr<UCG_DayManager> DayManagerSubsystem;
	UPROPERTY()
	TObjectPtr<UCG_QuestManager> QuestManagerSubsystem;
};
