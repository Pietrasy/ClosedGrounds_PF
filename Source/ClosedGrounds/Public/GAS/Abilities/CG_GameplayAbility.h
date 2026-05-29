// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CG_GameplayAbility.generated.h"

/**
 * Base class for abilities using in the project.
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCG_GameplayAbility();
};
