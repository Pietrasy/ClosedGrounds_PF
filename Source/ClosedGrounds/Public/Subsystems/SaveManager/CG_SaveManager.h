// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CG_SaveManager.generated.h"

class UCG_SaveGame;
/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_SaveManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCG_SaveGame> CurrentSaveGame;
	
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame();
};
