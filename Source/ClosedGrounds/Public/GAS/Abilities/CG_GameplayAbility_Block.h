// Pietras

#pragma once

#include "CoreMinimal.h"
#include "CG_GameplayAbility.h"
#include "CG_GameplayAbility_Block.generated.h"

/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_GameplayAbility_Block : public UCG_GameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> BlockAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> BlockEffectClass;
	
	FActiveGameplayEffectHandle ActiveBlockGameplayEffectHandle;
};
