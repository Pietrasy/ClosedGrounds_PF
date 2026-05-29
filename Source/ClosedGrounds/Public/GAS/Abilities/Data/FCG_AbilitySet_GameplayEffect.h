#pragma once

#include "FCG_AbilitySet_GameplayEffect.generated.h"

class UGameplayEffect;

USTRUCT()
struct FCG_AbilitySet_GameplayEffect
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;
};
