// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Data/FCG_DaySaveData.h"
#include "Data/FCG_GameSaveData.h"
#include "Data/FCG_QuestSaveData.h"
#include "GameFramework/SaveGame.h"
#include "CG_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_SaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	FCG_GameSaveData GameSaveData;
	
	UPROPERTY(EditDefaultsOnly)
	FCG_QuestSaveData QuestSaveData;
	
	UPROPERTY(EditDefaultsOnly)
	FCG_DaySaveData DaySaveData;
};
