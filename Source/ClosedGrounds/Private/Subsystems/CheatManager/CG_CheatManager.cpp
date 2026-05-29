// Pietras


#include "Subsystems/CheatManager/CG_CheatManager.h"

#include "AbilitySystemComponent.h"
#include "Characters/CG_PlayerCharacter.h"
#include "Game/CG_GameInstance.h"
#include "Game/CG_GameMode.h"
#include "GAS/AttributeSets/CG_CharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/QuestManager/CG_QuestManager.h"

void UCG_CheatManager::StartNewDay()
{
	const ACG_GameMode* GameMode = Cast<ACG_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->OnStartGame.Broadcast();
}

void UCG_CheatManager::FinishDay()
{
	UCG_GameInstance* GI = Cast<UCG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->GetSubsystem<UCG_QuestManager>()->CompleteDay();
}

void UCG_CheatManager::ForceCrash()
{
	check(false);
}

void UCG_CheatManager::SetWalkSpeed(float InWalkSpeed)
{
	UWorld* World = GetWorld();
	ACG_PlayerCharacter* PlayerChar = Cast<ACG_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	PlayerChar->AbilitySystemComponent->SetNumericAttributeBase(UCG_CharacterAttributeSet::GetMovementSpeedAttribute(), InWalkSpeed);
}

void UCG_CheatManager::ForceGarbageCollection()
{
	GEngine->ForceGarbageCollection();
}

void UCG_CheatManager::Suicide()
{
	ACG_GameMode* GameMode = Cast<ACG_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->HandlePlayerDeath();
}

void UCG_CheatManager::ForceSaveGame()
{
	UCG_GameInstance* GameInstance = Cast<UCG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//GameInstance->SaveGame();
}



