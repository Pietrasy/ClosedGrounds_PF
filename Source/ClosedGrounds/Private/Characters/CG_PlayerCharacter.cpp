// Pietras


#include "Characters/CG_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Widgets/CG_UIComponent.h"

ACG_PlayerCharacter::ACG_PlayerCharacter()
{
	// Create Player Camera.
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	// Player Camera rotates with player's rotation.
	PlayerCamera->bUsePawnControlRotation = true;
	// Attach PlayerComponent to player's root component.
	PlayerCamera->SetupAttachment(GetRootComponent());
	// Attach native Skeletal Mesh Component to Player Camera.
	GetMesh()->SetupAttachment(PlayerCamera);
	
	UIComponent = CreateDefaultSubobject<UCG_UIComponent>("UIComponent");
}

void ACG_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UIComponent->Initialize(AbilitySystemComponent);
}
