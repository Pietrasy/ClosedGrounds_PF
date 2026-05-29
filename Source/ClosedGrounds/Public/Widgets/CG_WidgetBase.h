// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CG_WidgetBase.generated.h"

class UCG_WidgetController;
/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_WidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UCG_WidgetController* InWidgetController);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCG_WidgetController> WidgetController;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
};
