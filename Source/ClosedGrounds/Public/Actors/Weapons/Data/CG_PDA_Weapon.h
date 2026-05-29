// Pietras

#pragma once

#include "CoreMinimal.h"
#include "FCG_WeaponParameters.h"
#include "FCG_WeaponProperties.h"
#include "Engine/DataAsset.h"
#include "CG_PDA_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_PDA_Weapon : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText WeaponName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCG_WeaponProperties Properties;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCG_WeaponParameters Parameters;
};
