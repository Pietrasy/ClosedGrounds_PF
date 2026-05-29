#pragma once
#include "GameplayTagContainer.h"

#include "FCG_AttackGameplayEffectWrapper.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FCG_AttackGameplayEffectWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUseSetByCaller = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bUseSetByCaller"))
	FGameplayTag CallerTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bUseSetByCaller"))
	float CallerValue = 0.f;
};
