// Pietras


#include "FunctionLibraries/CG_AssetManagerHelper.h"

#include "Engine/AssetManager.h"
#include "Subsystems/QuestManager/Data/CG_QuestData.h"

UCG_QuestData* UCG_AssetManagerHelper::GetQuestData(FPrimaryAssetId QuestID)
{
	return Cast<UCG_QuestData>(UAssetManager::Get().GetPrimaryAssetObject(QuestID));
}
