// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/Data/FCG_AbilitySet_GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "CG_AbilitySet.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS(Abstract)
class CLOSEDGROUNDS_API UCG_AbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	void AddToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Attribute Set", meta=(TitleProperty="AttributeSet"))
	TArray<TSubclassOf<UAttributeSet>> AttributeSetsToGrant;
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effect", meta=(TitleProperty="GameplayEffect"))
	TArray<FCG_AbilitySet_GameplayEffect> GameplayEffectsToGrant;
	
private:
	void GrantAttributeSets(UAbilitySystemComponent* AbilitySystemComponent);
	void GrantGameplayEffects(UAbilitySystemComponent* AbilitySystemComponent);
	
	virtual void GrantGameplayAbilities(UAbilitySystemComponent* AbilitySystemComponent) PURE_VIRTUAL();
};
