// Pietras


#include "GAS/Abilities/CG_GameplayAbility_Block.h"


#include "AbilitySystemComponent.h"
#include "ClosedGrounds/ClosedGrounds.h"

void UCG_GameplayAbility_Block::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	
	UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	
	if (AbilitySystemComponent == nullptr)
	{
		LOG_ERROR("[%hs] - AbilitySystemComponent isn't valid!", __FUNCTION__);
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
	
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(BlockEffectClass, 1.f, MakeEffectContext(Handle, ActorInfo));
	
	ActiveBlockGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
	
}

void UCG_GameplayAbility_Block::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveBlockGameplayEffectHandle);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
