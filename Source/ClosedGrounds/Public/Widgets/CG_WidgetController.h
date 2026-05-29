// Pietras

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CG_WidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedDynamic, float, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHungerChangedDynamic, float, Hunger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthInitializedDynamic, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHungerInitializedDynamic, float, MaxHealth);

class UCG_UIComponent;
/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_WidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(UCG_UIComponent* UIComponent);
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDynamic OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnHungerChangedDynamic OnHungerChanged;
	UPROPERTY(BlueprintAssignable)
	FOnHealthInitializedDynamic OnHealthInitialized;
	UPROPERTY(BlueprintAssignable)
	FOnHungerInitializedDynamic OnHungerInitialized;

private:
	void SetHealthPercent(float Value);
	void SetHungerPercent(float Value);
	
	void SetInitialHealth(float Value);
	void SetInitialHunger(float Value);
};
