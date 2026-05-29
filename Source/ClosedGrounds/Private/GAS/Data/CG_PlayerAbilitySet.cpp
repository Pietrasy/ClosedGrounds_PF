// Pietras


#include "GAS/Data/CG_PlayerAbilitySet.h"

#include "AbilitySystemComponent.h"
#include "GAS/Abilities/CG_GameplayAbility.h"
#include "GAS/Abilities/Data/FCG_AbilitySet_GameplayAbility.h"
#include "ClosedGrounds/ClosedGrounds.h"

void UCG_PlayerAbilitySet::GrantGameplayAbilities(UAbilitySystemComponent* AbilitySystemComponent)
{
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FCG_AbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.GameplayAbility))
		{
			UE_LOG(LogGAS, Error, TEXT("[%hs] GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), __FUNCTION__, AbilityIndex, *GetNameSafe(this));
			return;
		}
		
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityToGrant.GameplayAbility, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}
