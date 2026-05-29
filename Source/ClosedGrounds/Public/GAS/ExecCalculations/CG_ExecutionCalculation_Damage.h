// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CG_ExecutionCalculation_Damage.generated.h"

/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_ExecutionCalculation_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
