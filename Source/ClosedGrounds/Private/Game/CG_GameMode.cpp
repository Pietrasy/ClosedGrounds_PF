// Pietras


#include "Game/CG_GameMode.h"

#include "ClosedGrounds/ClosedGrounds.h"
#include "Subsystems/DayManager/CG_DayManager.h"
#include "Game/CG_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/QuestManager/CG_QuestManager.h"

void ACG_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	UGameInstance* GameInstance = GetGameInstance();
	
	DayManagerSubsystem = GameInstance->GetSubsystem<UCG_DayManager>();
	
	QuestManagerSubsystem = GameInstance->GetSubsystem<UCG_QuestManager>();
	
}

void ACG_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (QuestManagerSubsystem)
	{
		QuestManagerSubsystem->InitializeQuestSystem();
	}
	

}

void ACG_GameMode::StartPlay()
{
	Super::StartPlay();
	
	//DayManagerSubsystem->StartDay();
}

void ACG_GameMode::StartDay()
{
	OnStartGame.Broadcast();
	
	UCG_GameInstance* GameInstance = Cast<UCG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void ACG_GameMode::FinishDay()
{
	UCG_DayManager* DayManager = GetGameInstance()->GetSubsystem<UCG_DayManager>();
	UCG_QuestManager* QuestManager = GetGameInstance()->GetSubsystem<UCG_QuestManager>();
}

void ACG_GameMode::HandlePlayerDeath_Implementation()
{
	UCG_GameInstance* GameInstance = Cast<UCG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void ACG_GameMode::HandleLoadGame()
{
	StartDay();
}
