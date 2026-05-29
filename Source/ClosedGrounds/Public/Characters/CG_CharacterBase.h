// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GAS/Abilities/AbilitySets/CG_AbilitySet.h"
#include "CG_CharacterBase.generated.h"

/*
 *	Base class for all characters, such as: Player, Enemies, NPC.
 */

class UCG_GameplayAbility;
class UCG_WeaponComponent;
class UGC_HitDetectionComponent;
class UCG_CharacterAttributeSet;

struct FOnAttributeChangeData;

UCLASS(Abstract)
class CLOSEDGROUNDS_API ACG_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACG_CharacterBase();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category="WeaponSystem")
	TObjectPtr<UCG_WeaponComponent> WeaponComponent = nullptr;
	
	virtual void PossessedBy(AController* NewController) override;

	// Attribute set with vital attributes.
	UPROPERTY(BlueprintReadOnly, Category="AbilitySystem")
	TObjectPtr<UCG_CharacterAttributeSet> BasicAttributeSet = nullptr;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Damage")
	void HandleDeath();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HandleSpawn();
	
	virtual void InitialAttributes();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;
	
	UFUNCTION(BlueprintCallable, Category="AbilitySystem")
	TArray<FGameplayAbilitySpecHandle> GrantAbilities(TArray<TSubclassOf<UCG_GameplayAbility>> AbilitiesToGrant);

	UFUNCTION(BlueprintCallable, Category="AbilitySystem")
	void RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove);

	// Send the information about that the abilities have been changed.
	UFUNCTION(BlueprintCallable, Category="AbilitySystem")
	void SendAbilitiesChangedEvent();

	// Method from AbilitySystemInterface to retrieving Ability System Component.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void OnDeadTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsAlive;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCG_AbilitySet> AbilitySet;
	
	void OnMovementSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;
};
