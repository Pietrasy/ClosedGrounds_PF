#pragma once
#include "FCG_WeaponEffect.generated.h"

class UGameplayEffect;

USTRUCT(Blueprintable)
struct FCG_WeaponEffect
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Chance = 1.f;
};
