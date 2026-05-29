// Pietras


#include "GAS/Abilities/CG_GameplayAbility_Attack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAS/AttributeSets/CG_CharacterAttributeSet.h"
#include "ClosedGrounds/ClosedGrounds.h"
#include "GAS/Data/FCG_AttackGameplayEffectWrapper.h"
#include "Tags/CG_GAS_GameplayTags.h"

UCG_GameplayAbility_Attack::UCG_GameplayAbility_Attack()
{
	ActivationBlockedTags.AddTag(CG_GASGameplayTags::GAS_State_Dead);
	ActivationBlockedTags.AddTag(CG_GASGameplayTags::GAS_State_Stunned);
}

void UCG_GameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	
	SetAttackSpeed();
	
	UAnimMontage* AttackAnimMontage = GetComboAnimInstance();
	
	//UAbilityTask_PlayMontageAndWait* const Task_PlayMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(TEXT("AttackAbility")), AttackAnimMontage, AttackSpeed);

	if (0)
	{
		UAbilityTask_PlayMontageAndWait* const Task_PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(TEXT("AttackAbility")), AttackAnimMontage, AttackSpeed);
	}
	
	UAbilityTask_PlayMontageAndWait* const Task_PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(TEXT("AttackAbility")), AttackAnimMontage, 1);
		
	Task_PlayMontageAndWait->OnCompleted.AddDynamic(this, &UCG_GameplayAbility_Attack::OnMontageTaskFinished);
	Task_PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UCG_GameplayAbility_Attack::OnMontageTaskFinished);
	Task_PlayMontageAndWait->OnCancelled.AddDynamic(this, &UCG_GameplayAbility_Attack::OnMontageTaskFinished);
	Task_PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UCG_GameplayAbility_Attack::OnMontageTaskFinished);
	
	Task_PlayMontageAndWait->ReadyForActivation();
}

void UCG_GameplayAbility_Attack::ApplyAllEffectsToTarget(UAbilitySystemComponent* InAbilitySystemComponent) const
{
	if (!IsValid(InAbilitySystemComponent))
	{
		UE_LOG(LogGAS, Error, TEXT("[%hs] - InAbilitySystemComponent isn't valid!"), __FUNCTION__);
		return;
	}

	for (const FCG_AttackGameplayEffectWrapper& EffectWrapper : EffectToApplyOnHit)
	{
		FGameplayEffectContextHandle ContextHandle = MakeEffectContext(CurrentSpecHandle, CurrentActorInfo);
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(EffectWrapper.GameplayEffect);

		if (EffectWrapper.bUseSetByCaller)
		{
			const float EffectValue = EffectWrapper.CallerValue;
			EffectSpecHandle.Data->SetSetByCallerMagnitude(EffectWrapper.CallerTag, EffectValue);
		}
		
		InAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void UCG_GameplayAbility_Attack::OnMontageTaskFinished_Implementation()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UCG_GameplayAbility_Attack::SetAttackSpeed()
{
	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	const UCG_CharacterAttributeSet* CombatAttributeSet = AbilitySystemComponent->GetSet<UCG_CharacterAttributeSet>();

	if (!IsValid(CombatAttributeSet))
	{
		UE_LOG(LogGAS, Error, TEXT("[%hs] - CombatAttributeSet isn't valid!"), __FUNCTION__);
		return;
	}
	
	AttackSpeed = CombatAttributeSet->GetAttackSpeed();
}

UAnimMontage* UCG_GameplayAbility_Attack::GetComboAnimInstance() const
{
	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogGAS, Error, TEXT("[%hs] - World isn't valid!"), __FUNCTION__);
	}

	if (World->GetTimeSeconds() - LastAttackTime > ComboResetTime)
	{
		AnimationID = 0;
	}
	
	LastAttackTime = World->GetTimeSeconds();
	return AttackAnimations[(AnimationID++) % AttackAnimations.Num()];
}
