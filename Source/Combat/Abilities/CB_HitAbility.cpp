// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_HitAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UCB_HitAbility::UCB_HitAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCB_HitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilityTask_PlayMontageAndWait* PlayHitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, HitMontage);
	PlayHitTask->OnCompleted.AddDynamic(this, &UCB_HitAbility::OnCompleteCallback);
	PlayHitTask->OnInterrupted.AddDynamic(this, &UCB_HitAbility::OnCompleteCallback);
	PlayHitTask->ReadyForActivation();

	// AttackHitCue »£√‚
}

void UCB_HitAbility::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
