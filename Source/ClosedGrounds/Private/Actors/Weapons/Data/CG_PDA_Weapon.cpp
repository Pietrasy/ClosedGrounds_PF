// Pietras


#include "Actors/Weapons/Data/CG_PDA_Weapon.h"

FPrimaryAssetId UCG_PDA_Weapon::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Weapon", GetFName());
}
