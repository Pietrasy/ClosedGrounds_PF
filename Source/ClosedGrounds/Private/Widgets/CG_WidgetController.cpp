// Pietras


#include "Widgets/CG_WidgetController.h"

#include "Widgets/CG_UIComponent.h"

void UCG_WidgetController::Initialize(UCG_UIComponent* UIComponent)
{
	UIComponent->OnHealthChanged.AddUObject(this, &UCG_WidgetController::SetHealthPercent);
	UIComponent->OnHungerChanged.AddUObject(this, &UCG_WidgetController::SetHungerPercent);
	UIComponent->OnHealthInitialized.AddUObject(this, &UCG_WidgetController::SetInitialHealth);
	UIComponent->OnHungerInitialized.AddUObject(this, &UCG_WidgetController::SetInitialHunger);
}

void UCG_WidgetController::SetHealthPercent(float Value)
{
	OnHealthChanged.Broadcast(Value);
}

void UCG_WidgetController::SetHungerPercent(float Value)
{
	OnHungerChanged.Broadcast(Value); 
}

void UCG_WidgetController::SetInitialHealth(float Value)
{
	OnHealthInitialized.Broadcast(Value);
}

void UCG_WidgetController::SetInitialHunger(float Value)
{
	OnHungerInitialized.Broadcast(Value);
}
