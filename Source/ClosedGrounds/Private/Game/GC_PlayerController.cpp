// Pietras

#include "EnhancedInputSubsystems.h"
#include "Characters/CG_PlayerCharacter.h"
#include "Subsystems/CheatManager/CG_CheatManager.h"

#include "Game/CG_PlayerController.h"
#include "Widgets/CG_WidgetController.h"

ACG_PlayerController::ACG_PlayerController()
{
	CheatClass = UCG_CheatManager::StaticClass();
}

void ACG_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Check if a player is possessed.
	checkf(AcknowledgedPawn, TEXT("[%hs] - PlayerCharacter isn't valid!"), __FUNCTION__);
	PlayerCharacter = Cast<ACG_PlayerCharacter>(AcknowledgedPawn);

	// Check if an input context is set in BP_PlayerController.
	checkf(PlayerInputContext, TEXT("[%hs] - PlayerInputContext isn't valid!"), __FUNCTION__);
	EnablePlayerInput();
}

void ACG_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	WidgetController = NewObject<UCG_WidgetController>();
	WidgetController->Initialize(Cast<ACG_PlayerCharacter>(InPawn)->UIComponent);
	InitUI();
}

void ACG_PlayerController::EnablePlayerInput() const
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerInputContext, 0);
	}
}

void ACG_PlayerController::DisablePlayerInput() const
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(PlayerInputContext);
	}
}
