// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CG_AssetManagerHelper.generated.h"

class UCG_QuestData;
/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_AssetManagerHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static UCG_QuestData* GetQuestData(FPrimaryAssetId QuestID);
};
