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
	check(AcknowledgedPawn)
	PlayerCharacter = Cast<ACG_PlayerCharacter>(AcknowledgedPawn);

	// Check if an input context is set in BP_PlayerController.
	check(PlayerInputContext);
	// Retrieve the Enhanced Input System from the player.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// Add input mapping context to Enhanced Input System.
		Subsystem->AddMappingContext(PlayerInputContext, 0);
	}
}

void ACG_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	WidgetController = NewObject<UCG_WidgetController>();
	WidgetController->Initialize(Cast<ACG_PlayerCharacter>(InPawn)->UIComponent);
	InitUI();
}
