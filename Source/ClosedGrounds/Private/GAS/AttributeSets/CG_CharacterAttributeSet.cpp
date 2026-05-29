// Pietras


#include "GAS/AttributeSets/CG_CharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ClosedGrounds/ClosedGrounds.h"

#include "Tags/CG_GAS_GameplayTags.h"

UCG_CharacterAttributeSet::UCG_CharacterAttributeSet()
{
	// Set initial value of attributes.
	InitHealth(20.f);
	InitMaxHealth(20.f);
	InitHunger(100.f);
	InitMaxHunger(100.f);
	InitAttackSpeed(10.f);
	InitIncomingDamage(0.f);
	InitMovementSpeed(600.f);
	InitCritChance(0.f);
}

void UCG_CharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Clamp Health attribute.
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	// Clamp Stamina attribute.
	if (Attribute == GetHungerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHunger());
	}
}

void UCG_CharacterAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute() && Data.EvaluatedData.Magnitude != 0.f)
	{
		const float DealDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		
		// If the gameplay effect has the tag "HitReaction", activate it to the owner.
		if (Data.EffectSpec.GetDynamicAssetTags().HasTag(CG_GASGameplayTags::GAS_Effect_Hit))
		{
			FGameplayTagContainer HitReactionTagContainer;
			HitReactionTagContainer.AddTag(CG_GASGameplayTags::GAS_Ability_HitReaction);
			GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(HitReactionTagContainer);
		}
		SetHealth(GetHealth() - DealDamage);
	}

	// Check what an attribute is changing.
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Set a new value of the health attribute.
		SetHealth(GetHealth());
		
	} else if (Data.EvaluatedData.Attribute == GetHungerAttribute())
	{
		SetHunger(FMath::Clamp(GetHunger(), 0.0f, GetMaxHunger()));
	}
}

void UCG_CharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	// Check whether the health attribute is equal to zero. if so add the "Death" tag to the owner.
	if (Attribute == GetHealthAttribute() && NewValue <= 0.f)
	{
		FGameplayTagContainer DeathTagContainer;
		DeathTagContainer.AddTag(CG_GASGameplayTags::GAS_Ability_Die);
		GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(DeathTagContainer);
	}	
}


