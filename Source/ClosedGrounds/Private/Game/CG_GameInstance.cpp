// Pietras


#include "Game/CG_GameInstance.h"

#include "Subsystems/QuestManager/CG_QuestManager.h"

void UCG_GameInstance::Init()
{
	Super::Init();
	
	UCG_QuestManager* QuestManager = GetSubsystem<UCG_QuestManager>();
	checkf(QuestManager, TEXT("[%hs] - QuestManagerSubsystem isn't valid!"), __FUNCTION__);
	
	QuestManager->InitializeQuestSystem();
}