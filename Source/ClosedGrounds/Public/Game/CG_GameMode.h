// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CG_GameMode.generated.h"

/**
 * 
 */

class UCG_QuestManager;
class UCG_DayManager;

DECLARE_MULTICAST_DELEGATE(FOnStartGame)
DECLARE_MULTICAST_DELEGATE(FOnFinishDay)

UCLASS()
class CLOSEDGROUNDS_API ACG_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// POCZATEK ISTNIENIA
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	// ODPALA SIE Z INNYMI NIE GWARARNTUJE PIERWSZENSTWA
	virtual void BeginPlay() override;
	
	// ODPALA SIE PO WSZYSTKICH BEGIN PLAY ACTOROW
	virtual void StartPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void StartDay();
	void FinishDay();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandlePlayerDeath();
	
	UFUNCTION()
	void HandleLoadGame();
	
	FOnStartGame OnStartGame;
	FOnFinishDay OnFinishDay;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UMaterialInstance>> CloudsMaterials;
	
private:
	UPROPERTY()
	TObjectPtr<UCG_DayManager> DayManagerSubsystem;
	UPROPERTY()
	TObjectPtr<UCG_QuestManager> QuestManagerSubsystem;
	
	
};
