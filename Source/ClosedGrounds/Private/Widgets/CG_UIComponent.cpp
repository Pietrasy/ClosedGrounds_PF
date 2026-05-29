// Pietras


#include "Widgets/CG_UIComponent.h"

#include "AbilitySystemComponent.h"
#include "GAS/AttributeSets/CG_CharacterAttributeSet.h"

void UCG_UIComponent::Initialize(UAbilitySystemComponent* InASC)
{
	AbilitySystemComponent = InASC;
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCG_CharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UCG_UIComponent::HandleHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCG_CharacterAttributeSet::GetHungerAttribute()).AddUObject(this, &UCG_UIComponent::HandleHungerChanged);
	
	OnHealthInitialized.Broadcast(AbilitySystemComponent->GetNumericAttribute(UCG_CharacterAttributeSet::GetMaxHealthAttribute()));
	OnHungerInitialized.Broadcast(AbilitySystemComponent->GetNumericAttribute(UCG_CharacterAttributeSet::GetMaxHungerAttribute()));
}

void UCG_UIComponent::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	float MaxHealth = AbilitySystemComponent->GetNumericAttribute(UCG_CharacterAttributeSet::GetMaxHealthAttribute());
	
	float Percent = MaxHealth > 0 ? Health / MaxHealth : 0.f;
	
	OnHealthChanged.Broadcast(Percent);
}

void UCG_UIComponent::HandleHungerChanged(const FOnAttributeChangeData& Data)
{
	float Hunger = Data.NewValue;
	float MaxHunger = AbilitySystemComponent->GetNumericAttribute(UCG_CharacterAttributeSet::GetMaxHungerAttribute());
	
	float Percent = MaxHunger > 0 ? Hunger / MaxHunger : 0.f;
	
	OnHungerChanged.Broadcast(Percent);
}
