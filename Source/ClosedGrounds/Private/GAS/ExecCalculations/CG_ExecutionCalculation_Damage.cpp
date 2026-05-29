// Pietras


#include "GAS/ExecCalculations/CG_ExecutionCalculation_Damage.h"

#include "ClosedGrounds/ClosedGrounds.h"
#include "GAS/ExecCalculations/Data/FCG_DamageStatics.h"
#include "Tags/CG_GAS_GameplayTags.h"

static FCG_DamageStatics& DamageStatics()
{
	static FCG_DamageStatics DamageStatics;
	return DamageStatics;
}

void UCG_ExecutionCalculation_Damage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	
	const float CritChance = Spec.GetSetByCallerMagnitude(CG_GASGameplayTags::GAS_Data_Critical, false, 0.f);
	
	float CritChanceAttributeValue = 0.f;
	float DamageAttributeValue = 0.f;
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritChanceDef, EvaluateParameters, CritChanceAttributeValue);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluateParameters, DamageAttributeValue);

	if (FMath::FRand() <= CritChance)
	{
		DamageAttributeValue *= 2.f;
		
		FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();
		if (MutableSpec)
		{
			MutableSpec->AddDynamicAssetTag(CG_GASGameplayTags::GAS_Effect_CriticalAttack);
		}
		
		UE_LOG(LogGame, Log, TEXT("Critical Attack!"));
	}
	
	const float FinalDamage = DamageAttributeValue;
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCG_CharacterAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, FinalDamage));
}
