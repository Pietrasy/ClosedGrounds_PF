// Pietras


#include "Components/CG_WeaponComponent.h"

#include "AbilitySystemComponent.h"
#include "GAS/AttributeSets/CG_CharacterAttributeSet.h"
#include "Characters/CG_CharacterBase.h"
#include "ClosedGrounds/ClosedGrounds.h"
#include "Actors/Weapons/Data/CG_WeaponData.h"
#include "Actors/Weapons/Data/CG_PDA_Weapon.h"
#include "Actors/Weapons/Data/CG_Weapon.h"
#include "Tags/CG_GameplayTags.h"

void UCG_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheCharacterOwner();
}

void UCG_WeaponComponent::StartSetupWeapon()
{
	CacheCharacterOwner();
	AddWeapons();
}

void UCG_WeaponComponent::SetNewWeapon(UCG_PDA_Weapon* WeaponData)
{
	SpawnWeapon(WeaponData);
}

void UCG_WeaponComponent::CacheCharacterOwner()
{
	OwningCharacter = CastChecked<ACG_CharacterBase>(GetOwner());
}

void UCG_WeaponComponent::AddWeapons()
{
	if (IsValid(RightWeaponDataSTRUCT))
	{
		RightWeaponDataSTRUCT->Properties.HandTag = CG_GameplayTags::CG_WeaponHand_Right;
		RightWeapon = SpawnWeapon(RightWeaponDataSTRUCT);
	}
	
	if (RightWeaponData)
	{
		// Create an UCG_Weapon based on the data assigned to the WeaponComponent.
		//UCG_WeaponData* Weapon = NewObject<UCG_WeaponData>(this, RightWeaponData);
		// Assign a WeaponHand tag.
		//Weapon->HandTag = CG_GameplayTags::CG_WeaponHand_Right;
		//RightWeapon = SpawnWeapon(Weapon);
		RightWeapon = SpawnWeapon(RightWeaponDataSTRUCT);
	}
	
	if (LeftWeaponData)
	{
		UCG_WeaponData* Weapon = NewObject<UCG_WeaponData>(this, LeftWeaponData);
		Weapon->HandTag = CG_GameplayTags::CG_WeaponHand_Left;
		//LeftWeapon = SpawnWeapon(Weapon);
	}
}

TObjectPtr<ACG_Weapon> UCG_WeaponComponent::SpawnWeapon(TObjectPtr<UCG_PDA_Weapon> WeaponData)
{
	if (!IsValid(WeaponData))
	{
		UE_LOG(LogTemp, Error, TEXT("UCG_WeaponComponent::CreateWeaponFromData - WeaponData isn't valid!"));
		return nullptr;
	}

	if (IsValid(RightWeapon))
	{
		Weapons.Remove(RightWeapon);
		RightWeapon->Destroy();
	}
	// Spawn ACG_WeaponActor.
	ACG_Weapon* Weapon = GetWorld()->SpawnActor<ACG_Weapon>();
	// Populate the spawned weapon.
	// Attach to the hand of the character mesh.
	Weapon->SetupWeapon(OwningCharacter, WeaponData);
	AttachWeaponToHand(Weapon);
	// Add weapon abilities to the component owner.
	//OwningCharacter->GrantAbilities(WeaponData->Abilities);

	Weapon->HitEffectClass = DefaultHitEffect;
	
	// Null UCG_Weapon object.
	//WeaponData = nullptr;
	Weapons.Add(Weapon);

	return Weapon;
}

void UCG_WeaponComponent::AttachWeaponToHand(ACG_Weapon* WeaponActor) const
{
	// Attach a weapon to the correct hand, depending on a WeaponHand tag.
	if (WeaponActor->HandTag == CG_GameplayTags::CG_WeaponHand_Right)
	{
		WeaponActor->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightWeaponSocket);
	}
	else if (WeaponActor->HandTag == CG_GameplayTags::CG_WeaponHand_Left)
	{
		WeaponActor->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftWeaponSocket);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UCG_WeaponComponent::AttachWeaponToHand - Hand tag isn't valid!"))
	}
}

TObjectPtr<ACG_Weapon> UCG_WeaponComponent::GetWeaponInHand(const FGameplayTag HandTag)
{
	if (OwningCharacter == nullptr)
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("[%hs] - OwningCharacter isn't valid!"), __FUNCTION__);
		return nullptr;
	}
	
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