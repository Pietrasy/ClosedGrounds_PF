// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/CG_DayManagerInterface.h"
#include "CG_GameMode.generated.h"

class UCG_DayManagerComponent;
class UCG_QuestManager;

DECLARE_MULTICAST_DELEGATE(FOnStartGame);

UCLASS()
class CLOSEDGROUNDS_API ACG_GameMode : public AGameModeBase, public ICG_DayManagerInterface
{
	GENERATED_BODY()

public:
	FOnStartGame OnStartGame;
	
	UFUNCTION(BlueprintNativeEvent)
	void StartGame();
	
	ACG_GameMode();
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	//~~Begin ICG_DayManagerInterface
	virtual void RegisterSpawner(ACG_GameplayActor* InEnemySpawner) override;
	//~~End ICG_DayManagerInterface
	
	UCG_QuestManager* GetQuestManager() const;
	UCG_DayManagerComponent* GetDayManager() const;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCG_DayManagerComponent> DayManagerComponent;
	
private:
	UPROPERTY()
	TObjectPtr<UCG_QuestManager> QuestManager;
	
	
};
