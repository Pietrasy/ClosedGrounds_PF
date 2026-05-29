// Pietras

#pragma once

#include "CoreMinimal.h"
#include "CG_GameplayAbility.h"
#include "GAS/Data/FCG_AttackGameplayEffectWrapper.h"
#include "CG_GameplayAbility_Attack.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class CLOSEDGROUNDS_API UCG_GameplayAbility_Attack : public UCG_GameplayAbility
{
	GENERATED_BODY()
	
	
public:
	UCG_GameplayAbility_Attack();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable)
	void ApplyAllEffectsToTarget(UAbilitySystemComponent* InAbilitySystemComponent) const;
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnMontageTaskFinished();
	
	UFUNCTION(BlueprintGetter)
	UAnimMontage* GetComboAnimInstance() const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Attack")
	TArray<FCG_AttackGameplayEffectWrapper> EffectToApplyOnHit;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations")
	TArray<TObjectPtr<UAnimMontage>> AttackAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combo", meta=(ForceUnits="s", ClampMin="0.1"))
	float ComboResetTime = 3.f;
	
private:
	void SetAttackSpeed();
	
	float AttackSpeed = 1.f;
	
	static inline int32 AnimationID = 0;
	static inline int32 LastAttackTime = 0.f;
};
