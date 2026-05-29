// Pietras

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CG_CharacterAttributeSet.generated.h"

/**
 * Attribute set with vital attributes for characters.
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_CharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCG_CharacterAttributeSet();
	
	// === VITAL ===
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData Health;
	// Macro to create getters and setters for the attribute.
	ATTRIBUTE_ACCESSORS_BASIC(UCG_CharacterAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UCG_CharacterAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData Hunger;
	ATTRIBUTE_ACCESSORS_BASIC(UCG_CharacterAttributeSet, Hunger);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData MaxHunger;
	ATTRIBUTE_ACCESSORS_BASIC(UCG_CharacterAttributeSet, MaxHunger)
	
	// === COMBAT ===
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS_BASIC(UCG_CharacterAttributeSet, IncomingDamage);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS_BASIC(UCG_CharacterAttributeSet, AttackSpeed);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS_BASIC(UCG_CharacterAttributeSet, CritChance);
	
	// === Movement ===
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS_BASIC(UCG_CharacterAttributeSet, MovementSpeed);
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
};
