// Pietras

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CG_DayManagerInterface.generated.h"

class ACG_GameplayActor;
// This class does not need to be modified.
UINTERFACE()
class UCG_DayManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CLOSEDGROUNDS_API ICG_DayManagerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RegisterSpawner(ACG_GameplayActor* InEnemySpawner);
};
