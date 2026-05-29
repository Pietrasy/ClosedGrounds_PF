// Pietras

#include "Actors/Spawners/CG_SpawnerBase.h"

#include "Components/BillboardComponent.h"
#include "Components/TextRenderComponent.h"
#include "Subsystems/QuestManager/CG_QuestManager.h"

ACG_SpawnerBase::ACG_SpawnerBase()
{
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	
	if (IsValid(GetRootComponent()))
	{
		BillboardComponent->SetupAttachment(GetRootComponent());
	}
	else
	{
		SetRootComponent(BillboardComponent);
	}
}

void ACG_SpawnerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Tags.Add(UEnum::GetValueAsName(SpawnerType));
}

void ACG_SpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (bIsAlwaysSpawn)
	{
		Spawn();
	}
}

void ACG_SpawnerBase::OnInteracted()
{
	if (UCG_QuestManager* QM = GetGameInstance()->GetSubsystem<UCG_QuestManager>())
	{
		QM->NotifyObjective(SpawnerTag);
	}
}

void ACG_SpawnerBase::Spawn_Implementation()
{
	OnSpawn.Broadcast();
}
