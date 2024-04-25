// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_HitAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/CB_BaseCharacter.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Tags/StateTag.h"

UCB_HitAbility::UCB_HitAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCB_HitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	BaseCharacter = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());
	
	UAnimInstance* AnimInstance = BaseCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(HitMontage);

	PlayGameplayCue(TriggerEventData);	// 순서 바꾸니 해결
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	
	/*UAbilityTask_PlayMontageAndWait* PlayHitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,NAME_None, HitMontage);
	PlayHitTask->OnCompleted.AddDynamic(this, &UCB_HitAbility::OnCompleteCallback);
	PlayHitTask->OnBlendOut.AddDynamic(this, &UCB_HitAbility::OnBlendOutCallback);
	PlayHitTask->OnCancelled.AddDynamic(this, &UCB_HitAbility::OnCancelledCallback);
	PlayHitTask->OnInterrupted.AddDynamic(this, &UCB_HitAbility::OnInterruptedCallback);
	PlayHitTask->ReadyForActivation();*/
}

void UCB_HitAbility::PlayGameplayCue(const FGameplayEventData* TriggerEventData)
{
	const FGameplayAbilityTargetDataHandle& TargetDataHandle = TriggerEventData->TargetData;
	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(HitEffect);
	if (EffectSpecHandle.IsValid())
	{
		ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo,
			CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

		FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::
			GetEffectContext(EffectSpecHandle);
		CueContextHandle.AddHitResult(HitResult);
		FGameplayCueParameters CueParam;
		CueParam.EffectContext = CueContextHandle;

		BaseCharacter->GetAbilitySystemComponent()->ExecuteGameplayCue(GAMEPLAYCUE_ATTACKHIT, CueParam);
	}
}

void UCB_HitAbility::OnCompleteCallback()
{
	UE_LOG(LogTemp, Warning, TEXT("Hit : OnCompleteCallback"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCB_HitAbility::OnBlendOutCallback()
{
	UE_LOG(LogTemp, Warning, TEXT("Hit : OnBlendOutCallback"));
	//CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UCB_HitAbility::OnCancelledCallback()
{
	UE_LOG(LogTemp, Warning, TEXT("Hit : OnCancelledCallback"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UCB_HitAbility::OnInterruptedCallback()
{
	UE_LOG(LogTemp, Warning, TEXT("Hit : OnInterruptedCallback"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
