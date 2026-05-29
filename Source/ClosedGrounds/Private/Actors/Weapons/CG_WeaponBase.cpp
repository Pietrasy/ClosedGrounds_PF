// Pietras

#include "Actors/Weapons/CG_WeaponBase.h"


#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Actors/Weapons/Data/CG_PDA_Weapon.h"
#include "ClosedGrounds/ClosedGrounds.h"
#include "GameFramework/Character.h"
#include "GAS/ExecCalculations/CG_ExecutionCalculation_Damage.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tags/CG_GAS_GameplayTags.h"

void ACG_WeaponBase::SetUpWeapon(UCG_PDA_Weapon* Data)
{
	WeaponMesh->SetStaticMesh(Data->GetWeaponMesh());
	
	Properties = Data->Properties;
	Parameters = Data->Parameters;
}

// Sets default values
ACG_WeaponBase::ACG_WeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");

	if (IsValid(GetRootComponent()))
	{
		WeaponMesh->SetupAttachment(GetRootComponent());
	}
	else
	{
		SetRootComponent(WeaponMesh);
	}
	
	// Disable collision on the Static Mesh Component.
	WeaponMesh->SetCollisionProfileName("NoCollision", false);
}

void ACG_WeaponBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	PerformWeaponTrace();
}

void ACG_WeaponBase::CacheCurrentTopSocketLocation()
{
	if (IsValid(WeaponMesh))
	{
		SocketLocations.CurrentTopSocket = WeaponMesh->GetSocketLocation(TopSocketName);
	}
}

void ACG_WeaponBase::CacheCurrentBottomSocketLocation()
{
	if (IsValid(WeaponMesh))
	{
		SocketLocations.CurrentBottomSocket = WeaponMesh->GetSocketLocation(BottomSocketName);
	}
}

void ACG_WeaponBase::CachePreviousTopSocketLocation()
{
	if (IsValid(WeaponMesh))
	{
		SocketLocations.PreviousTopSocket = SocketLocations.CurrentTopSocket;
	}
}

void ACG_WeaponBase::CachePreviousBottomSocketLocation()
{
	if (IsValid(WeaponMesh))
	{
		SocketLocations.PreviousBottomSocket = SocketLocations.CurrentBottomSocket;
	}
}

void ACG_WeaponBase::ClearSocketLocations()
{
	SocketLocations.CurrentTopSocket = FVector3d::Zero();
	SocketLocations.CurrentBottomSocket = FVector3d::Zero();
	SocketLocations.PreviousTopSocket = FVector3d::Zero();
	SocketLocations.PreviousBottomSocket = FVector3d::Zero();
}

void ACG_WeaponBase::StartHitDetection()
{
	SocketLocations.PreviousTopSocket = WeaponMesh->GetSocketLocation(TopSocketName);
	SocketLocations.PreviousBottomSocket = WeaponMesh->GetSocketLocation(BottomSocketName);
	
	HitPawns.Reset();

	SetActorTickEnabled(true);
}

void ACG_WeaponBase::EndHitDirection()
{
	SetActorTickEnabled(false);
	ClearSocketLocations();
}

void ACG_WeaponBase::PerformWeaponTrace()
{
	// Set weapon sockets to current position.
	CacheCurrentTopSocketLocation();
	CacheCurrentBottomSocketLocation();

	// Create a query to ignore the player character.
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.AddIgnoredActor(GetInstigator());
	
	TArray<FHitResult> OutResults;

	// Determine a vector of weapon sockets.
	const FVector SocketsVector = (SocketLocations.CurrentTopSocket - SocketLocations.CurrentBottomSocket);
	// Calculate the distance between the sockets and divide it in half, because Unreal boxes use half-sizes (distance from the center of the box to one face).
	const float HalfLenght = SocketsVector.Size()*0.5f;
	// Create a shape based on the previous calculations.
	const FCollisionShape TraceBox = FCollisionShape::MakeBox(FVector3f(
		(HalfLenght),
		25.f,
		25.f));
	
	// Cast a multi-trace in the TraceBox's shape between the weapon sockets to detect Pawns.
	GetWorld()->SweepMultiByChannel(
		OutResults,
		SocketLocations.PreviousTopSocket,
		SocketLocations.CurrentTopSocket,
		UKismetMathLibrary::FindLookAtRotation(SocketLocations.CurrentTopSocket, SocketLocations.CurrentBottomSocket).Quaternion(),
		ECC_Pawn, 
		TraceBox,
		QueryParams);

	/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (false)
	{
		DrawDebugTraceBox(SocketLocations.CurrentTopSocket, SocketLocations.CurrentBottomSocket);
	}
	
	// Set current weapon sockets to previous position.
	CachePreviousTopSocketLocation();
	CachePreviousBottomSocketLocation();

	// For each hit result, check if the hit actor was a Pawn.
	for (FHitResult& Hit : OutResults)
	{
		
		APawn* HitPawn = Cast<APawn>(Hit.GetActor());
		if (HitPawn)
		{
			if (!HitPawns.Contains(HitPawn))
			{
				// Add a Pawn to HitPawns TSet.
				HitPawns.Add(HitPawn);
				DetectHitPawn(HitPawn);
			}
		}
	}
}

void ACG_WeaponBase::DrawDebugTraceBox(FVector TopSocket, FVector BottomSocket)
{
	const FVector Direction = (TopSocket - BottomSocket);
	const float HalfLenght = Direction.Size()*0.5f;
	const FVector Center = (TopSocket + BottomSocket) * 0.5f;
	const FQuat Rot = UKismetMathLibrary::FindLookAtRotation(TopSocket, BottomSocket).Quaternion();
	
	const float BoxDepth = 10.f;
	const float BoxHeight = 10.f;
	
	const FVector BoxExtent(HalfLenght,BoxDepth,BoxHeight);
	
	DrawDebugBox(
	GetWorld(),
	Center,
	BoxExtent,
	Rot,
	FColor::Blue,
	false,
	2.f,
	0,
	1.f
);
}

// @Deprecated
void ACG_WeaponBase::SetupWeapon(const TObjectPtr<ACharacter> InWeaponOwner, const UCG_PDA_Weapon* InWeaponData)
{
	WeaponOwner = InWeaponOwner;

	if (!IsValid(InWeaponData))
	{
		UE_LOG(LogGame, Error, TEXT("[%hs] - InWeaponData isn't valid!"), __FUNCTION__);
		return;
	}
	Parameters = InWeaponData->Parameters;
	Properties = InWeaponData->Properties;
	//WeaponMesh->SetStaticMesh(Properties.StaticMesh);
	
	UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(WeaponOwner);
	FGameplayEffectContextHandle EffectContext = OwnerASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	
	// Tworzenie uchwytu specyfikacji efektu (SpecHandle)
	// Używamy poziomu 1.0f (można to zmienić na poziom postaci/broni)
	FGameplayEffectSpecHandle NewHandle = OwnerASC->MakeOutgoingSpec(HitEffectClass, 1.0f, EffectContext);

	// Ustawienie uchwytu w broni za pomocą istniejącej funkcji
	SetEffectToWeapon(NewHandle);
}

void ACG_WeaponBase::SetEffectToWeapon(const FGameplayEffectSpecHandle& EffectSpec)
{
	EffectSpecHandleOld = EffectSpec;
}

void ACG_WeaponBase::DetectHitPawn(APawn* HitPawn)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitPawn);

	if (IsValid(TargetASC) && (WeaponOwner != HitPawn))
	{
		FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		ContextHandle.AddInstigator(WeaponOwner, this);
		
		//--- REFACTOR ---
		UGameplayEffect* DamageEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("InstantDamage")));
		DamageEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
		
		FGameplayEffectExecutionDefinition ExecutionDefinition;
		ExecutionDefinition.CalculationClass = UCG_ExecutionCalculation_Damage::StaticClass();
		DamageEffect->Executions.Add(ExecutionDefinition);
		
		FGameplayEffectSpec* InNewSpecHandle = new FGameplayEffectSpec(DamageEffect, ContextHandle, 1.f);
		InNewSpecHandle->SetSetByCallerMagnitude(CG_GASGameplayTags::GAS_Data_Damage, Parameters.Damage);
		InNewSpecHandle->SetSetByCallerMagnitude(CG_GASGameplayTags::GAS_Data_Critical, Parameters.CritChance);
		InNewSpecHandle->SetSetByCallerMagnitude(CG_GASGameplayTags::GAS_Cooldown_Attack, Parameters.AttackSpeed);
		InNewSpecHandle->AddDynamicAssetTag(CG_GASGameplayTags::GAS_Effect_Hit);
		//--- REFACTOR ---

		/*if (!IsValid(HitEffectClass))
		{
			UE_LOG(LogGame, Error, TEXT("[%hs] - HitEffectClass isn't valid!"), __FUNCTION__);
			return;
		}*/
		
		/*FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(HitEffectClass, 0.f, ContextHandle);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(CG_GASGameplayTags::GAS_Data_Damage, Parameters.Damage);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(CG_GASGameplayTags::GAS_Data_Critical, Parameters.CritChance);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(CG_GASGameplayTags::GAS_Cooldown_Attack, Parameters.AttackSpeed);
		SpecHandle.Data->AddDynamicAssetTag(CG_GASGameplayTags::GAS_Effect_Hit);*/

		//Apply weapon effects.
		for (const FCG_WeaponEffect& InWeaponEffect : WeaponEffects)
		{
			if (!IsValid(InWeaponEffect.EffectClass)) continue;
			
			float Roll = FMath::FRandRange(0.f,100.f);

			if (Roll <= InWeaponEffect.Chance)
			{
				FGameplayEffectContextHandle InContext = TargetASC->MakeEffectContext();
				ContextHandle.AddSourceObject(this);
				ContextHandle.AddInstigator(WeaponOwner, this);
				
				FGameplayEffectSpecHandle InSpecHandle = TargetASC->MakeOutgoingSpec(InWeaponEffect.EffectClass, 1.f, InContext);

				if (InSpecHandle.IsValid())
				{
					TargetASC->ApplyGameplayEffectSpecToSelf(*InSpecHandle.Data.Get());
				}
			}
		}
		
		TargetASC->ApplyGameplayEffectSpecToSelf(*InNewSpecHandle);
		
		UE_LOG(LogGame, Log, TEXT("[%s] is hit!"), *(TargetASC->GetAvatarActor()->GetName()));
	}
}
