// Pietras

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Actors/Weapons/FCG_WeaponEffect.h"
#include "Actors/Weapons/Data/FCG_WeaponParameters.h"
#include "Actors/Weapons/Data/FCG_WeaponProperties.h"
#include "GameFramework/Actor.h"
#include "CG_WeaponBase.generated.h"

class UCG_PDA_Weapon;
class UGameplayEffect;
struct FCG_WeaponParameters;
struct FGameplayEffectSpecHandle;

USTRUCT()
struct FWeaponSocketLocations
{
	GENERATED_BODY()
	
	FVector3d CurrentTopSocket = FVector3d::Zero();
	FVector3d PreviousTopSocket = FVector3d::Zero();
	FVector3d CurrentBottomSocket = FVector3d::Zero();
	FVector3d PreviousBottomSocket = FVector3d::Zero();
};

UCLASS()
class CLOSEDGROUNDS_API ACG_WeaponBase : public AActor
{
	GENERATED_BODY()
	//REFACTOR
public:
	FName WeaponName;
	void SetUpWeapon(UCG_PDA_Weapon* Data);
	//REFACTOR
	
public:
	ACG_WeaponBase();

	virtual void Tick(float DeltaSeconds) override;
	

	// Tag to indicate the hand that the weapon is held in.
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag WeaponHandTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> WeaponMesh = nullptr;

	UFUNCTION(BlueprintCallable)
	void SetEffectToWeapon(const FGameplayEffectSpecHandle& EffectSpec);

	// Struct holds the sockets location.
	FWeaponSocketLocations SocketLocations;

	// Functions manage socket locations.
	void CacheCurrentTopSocketLocation();
	void CacheCurrentBottomSocketLocation();
	void CachePreviousTopSocketLocation();
	void CachePreviousBottomSocketLocation();
	void ClearSocketLocations();
	
	// Methods that are endpoints for calls from an AnimWeaponNotifyState.
	void StartHitDetection();
	void EndHitDirection();
	
	TArray<FCG_WeaponEffect>  WeaponEffects;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> HitEffectClass;
	
	void SetupWeapon(TObjectPtr<ACharacter> InWeaponOwner, const UCG_PDA_Weapon* InWeaponData);
	
private:
	UPROPERTY()
	TObjectPtr<ACharacter> WeaponOwner;
	
	UPROPERTY()
	FCG_WeaponParameters Parameters;
	
	UPROPERTY()
	FCG_WeaponProperties Properties;
	
private:
	// Cast traces detect collisions with the weapon.
	void PerformWeaponTrace();
	// Debug depicting the trace from the PerformWeaponTrace().
	void DrawDebugTraceBox(FVector TopSocket, FVector BottomSocket);
	
	// Handle references to hit Pawns.
	void DetectHitPawn(APawn* HitPawn);

	// TSet is like a TArray but stores unique elements only. 
	TSet<TWeakObjectPtr<APawn>> HitPawns = {};
	
	FName TopSocketName = "Top";
	FName BottomSocketName = "Bottom";
	
	
	FGameplayEffectSpecHandle EffectSpecHandleOld = {};
};
