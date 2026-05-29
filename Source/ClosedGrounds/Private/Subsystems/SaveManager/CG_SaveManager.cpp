// Pietras


#include "Subsystems/SaveManager/CG_SaveManager.h"

#include "Kismet/GameplayStatics.h"
#include "Subsystems/DayManager/CG_DayManager.h"
#include "Subsystems/QuestManager/CG_QuestManager.h"
#include "Subsystems/SaveManager/CG_SaveGame.h"

void UCG_SaveManager::SaveGame()
{
	CurrentSaveGame = Cast<UCG_SaveGame>(UGameplayStatics::CreateSaveGameObject(UCG_SaveGame::StaticClass()));
		
	UCG_DayManager* DayManagerSubsystem = GetGameInstance()->GetSubsystem<UCG_DayManager>();
	
	UCG_QuestManager* QuestManagerSubsystem = GetGameInstance()->GetSubsystem<UCG_QuestManager>();
	
	CurrentSaveGame->DaySaveData = DayManagerSubsystem->GetDaySaveData();
	CurrentSaveGame->QuestSaveData = QuestManagerSubsystem->GetQuestSaveData();
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, TEXT("ChainSaveData"), 0);
}

void UCG_SaveManager::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("ChainSaveData"), 0))
	{
		CurrentSaveGame = Cast<UCG_SaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ChainSaveData"), 0));
	}
}
