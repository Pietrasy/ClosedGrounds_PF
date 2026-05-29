// Pietras


#include "Components/CG_WeaponComponent.h"

#include "AbilitySystemComponent.h"
#include "Characters/CG_CharacterBase.h"
#include "ClosedGrounds/ClosedGrounds.h"
#include "Actors/Weapons/Data/CG_PDA_Weapon.h"
#include "Actors/Weapons/CG_WeaponBase.h"
#include "Tags/CG_GameplayTags.h"



void UCG_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = CastChecked<ACG_CharacterBase>(GetOwner());
	AddWeaponToOwner(WeaponData);
}

void UCG_WeaponComponent::AddWeaponToOwner(UCG_PDA_Weapon* InWeaponData)
{
	if (!IsValid(InWeaponData))
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("[%hs] - WeaponData isn't valid!"), __FUNCTION__);
		return;
	}
	
	if (!IsValid(RightWeapon))
	{
		RightWeapon = SpawnWeapon();
	}
	
	RightWeapon->SetUpWeapon(InWeaponData);
	RightWeapon->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightWeaponSocket);
	
	if (InWeaponData->Properties.WeaponType == EWeaponType::DualWielding)
	{
		if (!IsValid(LeftWeapon))
		{
			LeftWeapon = SpawnWeapon();
		}
		
		LeftWeapon->SetUpWeapon(InWeaponData);
		LeftWeapon->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftWeaponSocket);
	}
}

ACG_WeaponBase* UCG_WeaponComponent::SpawnWeapon()
{
	UWorld* InWorld = GetWorld();
	checkf(InWorld, TEXT("[%hs] - World isn't valid!"), __FUNCTION__);
	
	ACG_WeaponBase* InSpawnedWeapon = GetWorld()->SpawnActor<ACG_WeaponBase>();
	InSpawnedWeapon->SetOwner(OwningCharacter);
	InSpawnedWeapon->SetInstigator(OwningCharacter);
	return InSpawnedWeapon;
}

void UCG_WeaponComponent::RemoveWeapons()
{
}

TArray<ACG_WeaponBase*> UCG_WeaponComponent::GetWeapons()
{
	TArray<ACG_WeaponBase*> Weapons;
	if (IsValid(RightWeapon))
	{
		Weapons.Add(RightWeapon);
	}
	if (IsValid(LeftWeapon))
	{
		Weapons.Add(LeftWeapon);
	}
		
	return Weapons;
}

TObjectPtr<ACG_WeaponBase> UCG_WeaponComponent::GetWeaponInHand(const FGameplayTag HandTag)
{
	if (HandTag == CG_GameplayTags::CG_WeaponHand_Right && IsValid(RightWeapon))
	{
		return  RightWeapon;
	}

	if (HandTag == CG_GameplayTags::CG_WeaponHand_Left && IsValid(LeftWeapon))
	{
		return LeftWeapon;
	}
	
	return nullptr;
}

void UCG_WeaponComponent::StartHitDetection(const FGameplayTag HandTag)
{
	if (!IsValid(GetWeaponInHand(HandTag))) return;
	GetWeaponInHand(HandTag)->StartHitDetection();
}

void UCG_WeaponComponent::EndHitDetection(const FGameplayTag HandTag)
{
	if (!IsValid(GetWeaponInHand(HandTag))) return;
	GetWeaponInHand(HandTag)->EndHitDirection();
}