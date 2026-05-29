// Pietras


#include "Notifies/GC_AnimWeaponNotifyState.h"

#include "Components/CG_WeaponComponent.h"

void UGC_AnimWeaponNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (IsValid(MeshComp->GetOwner()))
	{
		UCG_WeaponComponent* WComp = MeshComp->GetOwner()->GetComponentByClass<UCG_WeaponComponent>();
		if (IsValid(WComp))
		{
			WComp->StartHitDetection(HandTag);
		}
	}
}

void UGC_AnimWeaponNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (IsValid(MeshComp->GetOwner()))
	{
		UCG_WeaponComponent* WComp = MeshComp->GetOwner()->GetComponentByClass<UCG_WeaponComponent>();
		if (IsValid(WComp))
		{
			WComp->EndHitDetection(HandTag);
		}
	}
}