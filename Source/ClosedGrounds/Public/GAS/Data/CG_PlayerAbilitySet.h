// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/AbilitySets/CG_AbilitySet.h"
#include "CG_PlayerAbilitySet.generated.h"

struct FCG_AbilitySet_GameplayAbility;
/**
 * PO CO MI TO JESLI MAM ABILITYSET??????
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_PlayerAbilitySet : public UCG_AbilitySet
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effects", meta=(TitleProperty="GameplayAbility"))
	TArray<FCG_AbilitySet_GameplayAbility> GrantedGameplayAbilities;
	
private:
	virtual void GrantGameplayAbilities(UAbilitySystemComponent* AbilitySystemComponent) override;
};
