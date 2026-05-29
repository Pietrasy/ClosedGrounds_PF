// Pietras


#include "GAS/Abilities/CG_GameplayAbility_Heal.h"

#include "AbilitySystemComponent.h"
#include "ClosedGrounds/ClosedGrounds.h"

void UCG_GameplayAbility_Heal::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (!IsValid(HealGameplayEffect))
	{
		LOG_ERROR("[%hs] - HealGameplayEffect isn't valid!", __FUNCTION__);
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	if (AbilitySystemComponent == nullptr)
	{
		LOG_ERROR("[%hs] - AbilitySystemComponent isn't valid!", __FUNCTION__);
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
	
	FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(HealGameplayEffect, 1.f, MakeEffectContext(Handle, ActorInfo));
	
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
