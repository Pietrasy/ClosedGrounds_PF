// Pietras

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CG_SaveManagerInterface.generated.h"

class UCG_SaveGame;
// This class does not need to be modified.
UINTERFACE()
class UCG_SaveManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class CLOSEDGROUNDS_API ICG_SaveManagerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SaveData(UCG_SaveGame* InSaveGame) = 0;
	virtual void LoadData(const UCG_SaveGame* InSaveGame) = 0;
};
