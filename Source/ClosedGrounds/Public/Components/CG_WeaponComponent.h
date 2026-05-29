// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CG_WeaponComponent.generated.h"

class UGameplayEffect;
class UCG_PDA_Weapon;
class ACG_Weapon;
class UCG_WeaponData;
class ACG_CharacterBase;

UCLASS(Blueprintable, BlueprintType)
class CLOSEDGROUNDS_API UCG_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	void StartSetupWeapon();
	
	UFUNCTION(BlueprintCallable)
	void SetNewWeapon(UCG_PDA_Weapon* WeaponData);
	
	// Reference to owner this component.
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACG_CharacterBase> OwningCharacter = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DefaultHitEffect;

	// Members for weapon on each hand.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapons")
	TSubclassOf<UCG_WeaponData> RightWeaponData = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapons")
	TSubclassOf<UCG_WeaponData> LeftWeaponData = nullptr;
	
	// Members for weapon on each hand.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapons")
	TObjectPtr<UCG_PDA_Weapon> RightWeaponDataSTRUCT = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<ACG_Weapon>> Weapons = {};
	
	// Flag to enable draw debug hit boxes on a weapon. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponComponent|Debug")
	bool bDrawDebugHitBox = false;

	// TSet is like a TArray but stores unique elements only. 
	TSet<TWeakObjectPtr<APawn>> HitPawns;

	// This method should be called from the editor when a character is given weapons. This starts the logic to assign weapons to the WeaponComponent.
	UFUNCTION(BlueprintCallable)
	void AddWeapons();

	// Methods that are endpoints for calls from an AnimWeaponNotifyState.
	void StartHitDetection(FGameplayTag HandTag);
	void EndHitDetection(FGameplayTag HandTag);
	
private:
	// Cache reference to character owner this component.
	void CacheCharacterOwner();
	
	// Actors represent the weapons assigned to the WeaponComponent.
	UPROPERTY()
	TObjectPtr<ACG_Weapon> RightWeapon = nullptr;
	UPROPERTY()
	TObjectPtr<ACG_Weapon> LeftWeapon = nullptr;
	
	FName RightWeaponSocket = "Weapon_R";
	FName LeftWeaponSocket = "Weapon_L";

	// Create an ACG_WeaponActor based on the data from an UCG_Weapon object.
	TObjectPtr<ACG_Weapon> SpawnWeapon(TObjectPtr<UCG_PDA_Weapon> WeaponData);

	// Attach spawned an ACG_WeaponActor to a character mesh. 
	void AttachWeaponToHand(ACG_Weapon* WeaponActor) const;
	
	// Return an ACG_WeaponActor in the given hand.
	TObjectPtr<ACG_Weapon> GetWeaponInHand(FGameplayTag HandTag);
};
