// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Tags/CG_GameplayTags.h"
#include "UObject/Object.h"
#include "Actors/Weapons/FCG_WeaponEffect.h"
#include "CG_WeaponData.generated.h"

class UCG_GameplayAbility;
class UGameplayEffect;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class CLOSEDGROUNDS_API UCG_WeaponData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> WeaponMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> HitEffectClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackSpeed = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CritChance = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCG_WeaponEffect> WeaponEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UCG_GameplayAbility>> Abilities = {};

	// Gameplay tag to specify a hand in which the weapon is held.
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag HandTag = FGameplayTag::EmptyTag;
};
