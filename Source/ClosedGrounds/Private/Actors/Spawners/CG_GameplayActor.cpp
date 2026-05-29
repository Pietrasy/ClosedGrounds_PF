// Pietras

#include "Actors/GameplayActors/CG_GameplayActor.h"

#include "ClosedGrounds/ClosedGrounds.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/CG_DayManagerInterface.h"
#include "Kismet/GameplayStatics.h"

ACG_GameplayActor::ACG_GameplayActor()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GameplayActorMesh"));
	SetRootComponent(StaticMeshComponent);
}

ERegion ACG_GameplayActor::GetRegion() const
{
	return Region;
}

EGameplayActorType ACG_GameplayActor::GetType() const
{
	return Type;
}

void ACG_GameplayActor::BeginPlay()
{
	Super::BeginPlay();
	
	RegisterGameplayActor();
}

void ACG_GameplayActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	StaticMeshComponent->SetStaticMesh(GameplayActorMesh);
}

void ACG_GameplayActor::RegisterGameplayActor()
{
	if (!UGameplayStatics::GetGameMode(this)->Implements<UCG_DayManagerInterface>())
	{
		UE_LOG(LogDayManager, Error, TEXT("[%hs] - Game Mode doesn't implement DayManagerInterface"), __FUNCTION__);
	}
	 Cast<ICG_DayManagerInterface>(UGameplayStatics::GetGameMode(this))->RegisterSpawner(this);
}

void ACG_GameplayActor::Activate_Implementation()
{
	OnActivate.Broadcast();
}

void ACG_GameplayActor::Deactivate_Implementation()
{
	OnDeactivate.Broadcast();
}