#pragma once
#include "GameplayTagContainer.h"
#include "Data/ECG_WeaponType.h"

#include "FCG_WeaponProperties.generated.h"

class UCG_GameplayAbility;
struct FCG_WeaponEffect;

USTRUCT(BlueprintType)
struct FCG_WeaponProperties
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType = EWeaponType::OneHanded;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCG_WeaponEffect> GameplayEffects = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UCG_GameplayAbility>> Abilities = {};
};
