#pragma once
#include "GameplayTagContainer.h"

#include "FCG_WeaponProperties.generated.h"

class UCG_GameplayAbility;
struct FCG_WeaponEffect;

USTRUCT(BlueprintType)
struct FCG_WeaponProperties
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> WeaponMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCG_WeaponEffect> WeaponEffects = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UCG_GameplayAbility>> Abilities = {};

	// Gameplay tag to specify a hand in which the weapon is held.
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag HandTag = FGameplayTag::EmptyTag;
};
