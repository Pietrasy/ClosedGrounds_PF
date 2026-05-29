// Pietras


#include "Actors/Weapons/Data/CG_PDA_Weapon.h"

FPrimaryAssetId UCG_PDA_Weapon::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Weapon", GetFName());
}

FText UCG_PDA_Weapon::GetWeaponName()
{
	return Name;
}

UStaticMesh* UCG_PDA_Weapon::GetWeaponMesh()
{
	return StaticMesh;
}

TArray<FCG_WeaponEffect> UCG_PDA_Weapon::GetWeaponEffects()
{
	return Properties.GameplayEffects;
}

TArray<TSubclassOf<UCG_GameplayAbility>> UCG_PDA_Weapon::GetWeaponAbilities()
{
	return Properties.Abilities;
}

float UCG_PDA_Weapon::GetWeaponDamage() const
{
	return Parameters.Damage;
}

float UCG_PDA_Weapon::GetWeaponAttackSpeed() const
{
	return Parameters.AttackSpeed;
}

float UCG_PDA_Weapon::GetWeaponCritChance() const
{
	return Parameters.CritChance;
}

EWeaponType UCG_PDA_Weapon::GetWeaponType() const
{
	return Properties.WeaponType;
}
