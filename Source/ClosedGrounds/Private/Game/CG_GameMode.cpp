// Pietras


#include "Game/CG_GameMode.h"

#include "CG_ClosedGroundsSettings.h"
#include "ClosedGrounds/ClosedGrounds.h"
#include "Components/CG_DayManagerComponent.h"
#include "Subsystems/QuestManager/CG_QuestManager.h"


ACG_GameMode::ACG_GameMode()
{
	DayManagerComponent = CreateDefaultSubobject<UCG_DayManagerComponent>(TEXT("DayManagerComponent"));
}

void ACG_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	const UGameInstance* GameInstance = GetGameInstance();
	const UCG_ClosedGroundsSettings* ClosedGroundsSettings = GetDefault<UCG_ClosedGroundsSettings>();
	
	QuestManager = GameInstance->GetSubsystem<UCG_QuestManager>();
	QuestManager->SetNumQuestsPerDay(ClosedGroundsSettings->NumberQuestsPerDay);
	
	DayManagerComponent->OnInitializedDay.BindUObject(this, &ACG_GameMode::StartGame);
	DayManagerComponent->SetVolumetricClouds(ClosedGroundsSettings->Clouds);
	DayManagerComponent->InitializeDayManager();
}

void ACG_GameMode::RegisterSpawner(ACG_GameplayActor* InEnemySpawner)
{
	ICG_DayManagerInterface::RegisterSpawner(InEnemySpawner);
	
	DayManagerComponent->RegisterSpawner(InEnemySpawner);
}

void ACG_GameMode::StartGame_Implementation()
{
	DayManagerComponent->SetPlayerSpawn();
	OnStartGame.Broadcast();
}


UCG_QuestManager* ACG_GameMode::GetQuestManager() const
{
	if (!IsValid(QuestManager))
	{
		UE_LOG(LogGame, Error, TEXT("[%hs] - QuestManager isn't valid!"), __FUNCTION__);
		return nullptr;
	}
	
	return QuestManager;
}

UCG_DayManagerComponent* ACG_GameMode::GetDayManager() const
{
	if (!IsValid(DayManagerComponent))
	{
		UE_LOG(LogGame, Error, TEXT("[%hs] - DayManagerComponent isn't valid!"), __FUNCTION__);
		return nullptr;
	}
	
	return DayManagerComponent;
}
