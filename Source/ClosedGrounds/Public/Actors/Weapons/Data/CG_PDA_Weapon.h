// Pietras

#pragma once

#include "CoreMinimal.h"
#include "FCG_WeaponParameters.h"
#include "FCG_WeaponProperties.h"
#include "Actors/Weapons/FCG_WeaponEffect.h"
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
	FText Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> StaticMesh = nullptr;
	
	UFUNCTION(BlueprintCallable, Category="WeaponData")
	FText GetWeaponName();
	
	UFUNCTION(BlueprintCallable, Category="WeaponData")
	UStaticMesh* GetWeaponMesh();
	
	UFUNCTION(BlueprintCallable, Category="WeaponData")
	TArray<FCG_WeaponEffect> GetWeaponEffects();
	
	UFUNCTION(BlueprintCallable, Category="WeaponData")
	TArray<TSubclassOf<UCG_GameplayAbility>> GetWeaponAbilities();
	
	UFUNCTION(BlueprintCallable, Category="WeaponData")
	float GetWeaponDamage() const;
	
	UFUNCTION(BlueprintCallable, Category="WeaponData")
	float GetWeaponAttackSpeed() const;
	
	UFUNCTION(BlueprintCallable, Category="WeaponData")
	float GetWeaponCritChance() const;
	
	UFUNCTION(BlueprintCallable, Category="WeaponData")
	EWeaponType GetWeaponType() const;
	
	UPROPERTY(EditDefaultsOnly)
	FCG_WeaponProperties Properties;
	
	UPROPERTY(EditDefaultsOnly)
	FCG_WeaponParameters Parameters;
};
