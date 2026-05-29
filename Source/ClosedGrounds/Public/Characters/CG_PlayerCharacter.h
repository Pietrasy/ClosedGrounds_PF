// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/CG_CharacterBase.h"
#include "CG_PlayerCharacter.generated.h"

class UCG_UIComponent;
class UCameraComponent;
class UCG_WeaponComponent;

UCLASS()
class CLOSEDGROUNDS_API ACG_PlayerCharacter : public ACG_CharacterBase
{
	GENERATED_BODY()

public:
	ACG_PlayerCharacter();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<UCameraComponent> PlayerCamera = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCG_UIComponent> UIComponent;
};
