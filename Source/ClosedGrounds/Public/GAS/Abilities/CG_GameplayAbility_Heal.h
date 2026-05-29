// Pietras

#pragma once

#include "CoreMinimal.h"
#include "CG_GameplayAbility.h"
#include "CG_GameplayAbility_Heal.generated.h"

/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_GameplayAbility_Heal : public UCG_GameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> HealGameplayEffect;
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
