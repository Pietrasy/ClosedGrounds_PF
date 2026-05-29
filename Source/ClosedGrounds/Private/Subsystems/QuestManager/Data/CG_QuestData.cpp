// Pietras


#include "Subsystems/QuestManager/Data/CG_QuestData.h"

FPrimaryAssetId UCG_QuestData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Quest", GetFName());
}
