// Pietras


#include "GAS/Abilities/CG_GameplayAbility.h"

#include "Tags/CG_GAS_GameplayTags.h"

UCG_GameplayAbility::UCG_GameplayAbility()
{
	// Add the "Active" tag at the start of an ability.
	ActivationOwnedTags.AddTag(CG_GASGameplayTags::GAS_Ability_Active);
	// Block an ability if an ASC owner has the "Dead" tag.
	ActivationBlockedTags.AddTag(CG_GASGameplayTags::GAS_State_Dead);
}
