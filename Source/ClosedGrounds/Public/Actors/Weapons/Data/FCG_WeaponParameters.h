#pragma once

#include "FCG_WeaponParameters.generated.h"

USTRUCT(BlueprintType)
struct FCG_WeaponParameters
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackSpeed = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CritChance = 0.f;
};
