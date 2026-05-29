// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CG_PlayerController.generated.h"

class UCG_WidgetController;
class ACG_PlayerCharacter;
class UInputMappingContext;

UCLASS()
class CLOSEDGROUNDS_API ACG_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACG_PlayerController();
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;
	
	void EnablePlayerInput() const;
	void DisablePlayerInput()const ;
	

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACG_PlayerCharacter> PlayerCharacter = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCG_WidgetController> WidgetController;
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitUI();

private:
    // Member for custom Input Mapping Context.
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> PlayerInputContext;
};