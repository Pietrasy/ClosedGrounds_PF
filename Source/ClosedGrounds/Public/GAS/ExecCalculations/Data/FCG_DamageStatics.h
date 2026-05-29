#pragma once
#include "GameplayEffectAttributeCaptureDefinition.h"
#include "GAS/AttributeSets/CG_CharacterAttributeSet.h"

#include "FCG_DamageStatics.generated.h"

class UCG_CharacterAttributeSet;

USTRUCT()
struct FCG_DamageStatics
{
	GENERATED_BODY()
	
	FGameplayEffectAttributeCaptureDefinition DamageDef;
	FGameplayEffectAttributeCaptureDefinition CritChanceDef;
	
	FCG_DamageStatics()
	{
		DamageDef = FGameplayEffectAttributeCaptureDefinition(UCG_CharacterAttributeSet::GetIncomingDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		CritChanceDef = FGameplayEffectAttributeCaptureDefinition(UCG_CharacterAttributeSet::GetCritChanceAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};
