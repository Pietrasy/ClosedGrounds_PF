// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CG_WeaponComponent.generated.h"

class UGameplayEffect;
class UCG_PDA_Weapon;
class ACG_WeaponBase;
class UCG_WeaponData;
class ACG_CharacterBase;

UCLASS(Blueprintable, BlueprintType)
class CLOSEDGROUNDS_API UCG_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	/**
	 * @brief Create a weapon actor if one doesn't already exist and populates it with data.
	 * @param InWeaponData Weapon data.
	 */
	void AddWeaponToOwner(UCG_PDA_Weapon* InWeaponData);

	/**
	 * @brief Spawns a weapon actor.
	 * @return Weapon actor with Owner and Instigator set to OwningCharacter.
	 */
	ACG_WeaponBase* SpawnWeapon();
	
	UFUNCTION(BlueprintCallable)
	TArray<ACG_WeaponBase*> GetWeapons();
	
	void RemoveWeapons();
	
	// Methods that are endpoints for calls from an AnimWeaponNotifyState.
	void StartHitDetection(FGameplayTag HandTag);
	void EndHitDetection(FGameplayTag HandTag);

public:
	// Reference to the owner this component.
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACG_CharacterBase> OwningCharacter = nullptr;
	// Members for weapon on each hand.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapons")
	TObjectPtr<UCG_PDA_Weapon> WeaponData = nullptr;
	
	// Flag to enable draw debug hit boxes on a weapon. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponComponent|Debug")
	bool bDrawDebugHitBox = false;

private:
	UPROPERTY()
	TObjectPtr<ACG_WeaponBase> RightWeapon;
	UPROPERTY()
	TObjectPtr<ACG_WeaponBase> LeftWeapon;
	
	FName RightWeaponSocket = "Weapon_R";
	FName LeftWeaponSocket = "Weapon_L";
	
	// Return an ACG_WeaponActor in the given hand.
	TObjectPtr<ACG_WeaponBase> GetWeaponInHand(FGameplayTag HandTag);
};
