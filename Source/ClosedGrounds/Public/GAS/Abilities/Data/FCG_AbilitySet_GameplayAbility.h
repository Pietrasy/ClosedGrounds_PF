#pragma once

#include "FCG_AbilitySet_GameplayAbility.generated.h"

class UCG_GameplayAbility;

USTRUCT()
struct FCG_AbilitySet_GameplayAbility
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCG_GameplayAbility> GameplayAbility = nullptr;
};
