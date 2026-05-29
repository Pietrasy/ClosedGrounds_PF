// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CG_ClosedGroundsSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Closed Grounds"))
class CLOSEDGROUNDS_API UCG_ClosedGroundsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="DaySettings")
	int32 NumberQuestsPerDay = 2;
	
	UPROPERTY(Config, EditAnywhere, Category="DaySettings")
	TArray<TSoftObjectPtr<UMaterialInstance>> Clouds;
};
