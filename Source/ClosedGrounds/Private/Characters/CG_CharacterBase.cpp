// Pietras

#include "Characters/CG_CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/Abilities/CG_GameplayAbility.h"
#include "GAS/AttributeSets/CG_CharacterAttributeSet.h"
#include "ClosedGrounds/ClosedGrounds.h"
#include "Components/CG_WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tags/CG_GAS_GameplayTags.h"

ACG_CharacterBase::ACG_CharacterBase()
{
	// Disable calling tick in every frame.
	PrimaryActorTick.bCanEverTick = false;
	// Create weapon component.
	WeaponComponent = CreateDefaultSubobject<UCG_WeaponComponent>("WeaponComponent");
	// Create Ability System Component.
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	// Create Basic Attribute Set.
	BasicAttributeSet = CreateDefaultSubobject<UCG_CharacterAttributeSet>("BasicAttributeSet");
	bIsAlive = true;
}

void ACG_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// Listen for whether the owner has received the "Dead" tag, then call the callback function.
	AbilitySystemComponent->RegisterGameplayTagEvent(CG_GASGameplayTags::GAS_State_Dead).AddUObject(this, &ACG_CharacterBase::OnDeadTagChanged);
	
	
	checkf(AbilitySystemComponent, TEXT("AbilitySystemComponent - isn't valid!"));
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (IsValid(AbilitySet))
	{
		AbilitySet->AddToAbilitySystem(AbilitySystemComponent);
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("[%hs] - AbilitySet isn't valid!"), __FUNCTION__);
	}
	
	const UCG_CharacterAttributeSet* const MovementAttributeSet = AbilitySystemComponent->GetSet<UCG_CharacterAttributeSet>();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MovementAttributeSet->GetMovementSpeedAttribute()).AddUObject(this, &ACG_CharacterBase::OnMovementSpeedChanged);
	WeaponComponent->StartSetupWeapon();
}

void ACG_CharacterBase::OnDeadTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// Check if a tag was added.
	if (NewCount)
	{
		// Use C++, not blueprint implementation.
		HandleDeath();
	}
}

void ACG_CharacterBase::OnMovementSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	UCharacterMovementComponent* const CharacterMovementComponent = GetCharacterMovement();
	if (!IsValid(CharacterMovementComponent))
	{
		UE_LOG(LogGame, Error, TEXT("[%hs] - CharacterMovementComponent"), __FUNCTION__);
	}
	
	const float NewMovementSpeed = FMath::Max(OnAttributeChangeData.NewValue, 0.f);
	CharacterMovementComponent->MaxWalkSpeed = NewMovementSpeed;
}

UAbilitySystemComponent* ACG_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACG_CharacterBase::HandleDeath_Implementation()
{
	// Function to override in blueprints.
	bIsAlive = false;
	UE_LOG(LogGame, Log, TEXT("[%s] is dead!"), *(this->GetName()));
}

void ACG_CharacterBase::HandleSpawn_Implementation()
{
	// Function to override in blueprints.
	
	FGameplayEffectQuery Query;
	AbilitySystemComponent->RemoveActiveEffects(Query);
	InitialAttributes();
	
	bIsAlive = true;
	UE_LOG(LogGame, Log, TEXT("[%s] is alive!"), *(this->GetName()));
}

void ACG_CharacterBase::InitialAttributes()
{
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	if (!IsValid(DefaultAttributeEffect))
	{
		return;
	}
	
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1.f, ContextHandle);
	
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

TArray<FGameplayAbilitySpecHandle> ACG_CharacterBase::GrantAbilities(TArray<TSubclassOf<UCG_GameplayAbility>> AbilitiesToGrant)
{
	// Check whether the owner has the ASC. If not, return an empty array.
	if (!AbilitySystemComponent) return TArray<FGameplayAbilitySpecHandle>();

	TArray<FGameplayAbilitySpecHandle> AbilityHandles;

	// Add abilities to the character.
	for (TSubclassOf<UCG_GameplayAbility> Ability : AbilitiesToGrant)
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, -1, this));
		AbilityHandles.Add(SpecHandle);
	}
	
	SendAbilitiesChangedEvent();
	return AbilityHandles;
}

void ACG_CharacterBase::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove)
{
	if (!AbilitySystemComponent) return;

	// Clear abilities from the character.
	for (auto AbilityHandle : AbilityHandlesToRemove)
	{
		AbilitySystemComponent->ClearAbility(AbilityHandle);
	}

	SendAbilitiesChangedEvent();
}

void ACG_CharacterBase::SendAbilitiesChangedEvent()
{
	FGameplayEventData EventData;
	EventData.EventTag = CG_GASGameplayTags::GAS_Event_Abilities_Change;
	EventData.Instigator = this;
	EventData.Target = this;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);
}