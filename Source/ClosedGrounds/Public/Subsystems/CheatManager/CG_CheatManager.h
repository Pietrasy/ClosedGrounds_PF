// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "CG_CheatManager.generated.h"

/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_CheatManager : public UCheatManager
{
	GENERATED_BODY()

	UFUNCTION(Exec, BlueprintCallable)
	void StartNewDay();
	
	UFUNCTION(Exec, BlueprintCallable)
	void FinishDay();

	UFUNCTION(Exec, BlueprintCallable)
	void ForceCrash();
	
	UFUNCTION(Exec, BlueprintCallable)
	void SetWalkSpeed(float InWalkSpeed);

	UFUNCTION(Exec, BlueprintCallable)
	void ForceGarbageCollection();
	
	UFUNCTION(Exec, BlueprintCallable)
	void ForceSaveGame();
};
