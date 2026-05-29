// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CG_UIComponent.generated.h"

class UAbilitySystemComponent;
struct FOnAttributeChangeData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHungerChangedSignature, float)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthInitializedSignature, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHungerInitializedSignature, float);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CLOSEDGROUNDS_API UCG_UIComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	void Initialize(UAbilitySystemComponent* InASC);
	
	FOnHealthChangedSignature OnHealthChanged;
	FOnHungerChangedSignature OnHungerChanged;
	FOnHealthInitializedSignature OnHealthInitialized;
	FOnHungerInitializedSignature OnHungerInitialized;
	
private:
	void HandleHealthChanged(const FOnAttributeChangeData& Data);
	void HandleHungerChanged(const FOnAttributeChangeData& Data);
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
