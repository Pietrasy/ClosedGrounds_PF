// Pietras


#include "GAS/Abilities/AbilitySets/CG_AbilitySet.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ClosedGrounds/ClosedGrounds.h"

void UCG_AbilitySet::AddToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent)
{
	check(AbilitySystemComponent);
	
	GrantAttributeSets(AbilitySystemComponent);
	GrantGameplayEffects(AbilitySystemComponent);
	GrantGameplayAbilities(AbilitySystemComponent);
}

void UCG_AbilitySet::GrantAttributeSets(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AttributeSetsToGrant.IsEmpty())
	{
		UE_LOG(LogGAS, Error, TEXT("[%hs] - AttributeSetsToGrant is empty!"), __FUNCTION__);
		return;
	}
	
	for (int32 SetIndex = 0; SetIndex < AttributeSetsToGrant.Num(); ++SetIndex)
	{
		if (AttributeSetsToGrant.IsValidIndex(SetIndex)) 
		{
			return;
		}
		
		const TSubclassOf<UAttributeSet>& AttributeSetToGrant = AttributeSetsToGrant[SetIndex];
		
		UAttributeSet* NewSet = NewObject<UAttributeSet>(AbilitySystemComponent->GetOwner(), AttributeSetToGrant);
		
		AbilitySystemComponent->AddAttributeSetSubobject(NewSet);
	}
	
	UE_LOG(LogGAS, Log, TEXT("Added %d attribute sets"), AttributeSetsToGrant.Num());
}

void UCG_AbilitySet::GrantGameplayEffects(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (GameplayEffectsToGrant.IsEmpty())
	{
		return;
	}
	
	for (int32 EffectIndex = 0; EffectIndex < GameplayEffectsToGrant.Num(); ++EffectIndex)
	{
		const FCG_AbilitySet_GameplayEffect& GameplayEffectToGrant = GameplayEffectsToGrant[EffectIndex];

		if (!IsValid(GameplayEffectToGrant.GameplayEffect))
		{
			UE_LOG(LogGAS, Error, TEXT("[%hs] GameplayEffectToGrants[%d] on ability set [%s] isn't valid!"), __FUNCTION__, EffectIndex, *GetNameSafe(this));
			continue;
		}
		
		const UGameplayEffect* GameplayEffect = GameplayEffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, 1.f, AbilitySystemComponent->MakeEffectContext());
	}
}
