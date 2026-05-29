// Pietras


#include "Game/CG_GameInstance.h"

#include "ClosedGrounds/ClosedGrounds.h"
#include "Subsystems/DayManager/CG_DayManager.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/QuestManager/CG_QuestManager.h"
#include "Subsystems/SaveManager/CG_SaveGame.h"

void UCG_GameInstance::Init()
{
	Super::Init();
	
	
	//PO CO TEN CACHE?
	DayManagerSubsystem = GetSubsystem<UCG_DayManager>();
	checkf(DayManagerSubsystem, TEXT("[%hs] - DayManagerSubsystem isn't valid!"), __FUNCTION__);
	
	QuestManagerSubsystem = GetSubsystem<UCG_QuestManager>();
	checkf(QuestManagerSubsystem, TEXT("[%hs] - QuestManagerSubsystem isn't valid!"), __FUNCTION__);
}