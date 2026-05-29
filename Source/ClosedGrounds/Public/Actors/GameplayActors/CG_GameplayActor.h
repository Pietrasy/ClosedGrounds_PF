// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Data/ECG_Region.h"
#include "Data/ECG_GameplayActorType.h"
#include "GameFramework/Actor.h"
#include "CG_GameplayActor.generated.h"

class UTextRenderComponent;
class UBillboardComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeactivate);

UCLASS(Abstract)
class CLOSEDGROUNDS_API ACG_GameplayActor : public AActor
{
	GENERATED_BODY()

public:
	ACG_GameplayActor();
	
	virtual void BeginPlay() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="GameplayActor")
	FOnActivate OnActivate;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="GameplayActor")
	FOnDeactivate OnDeactivate;
	
	UPROPERTY(EditAnywhere, Category="GameplayActor")
	TObjectPtr<UStaticMesh> GameplayActorMesh;
	
	UFUNCTION(BlueprintCallable, BlueprintGetter, Category="GameplayActor")
	ERegion GetRegion() const;

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category="GameplayActor")
	EGameplayActorType GetType() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void Activate();
	
	UFUNCTION(BlueprintNativeEvent)
	void Deactivate();

protected:
	UPROPERTY(BlueprintReadOnly,Category="GameplayActor")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category="GameplayActor")
	ERegion Region = ERegion::Null;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="GameplayActor")
	EGameplayActorType Type;
	
private:
	void RegisterGameplayActor();
};
