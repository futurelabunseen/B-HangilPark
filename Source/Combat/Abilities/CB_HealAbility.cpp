// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CB_HealAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/CB_BaseCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UCB_HealAbility::UCB_HealAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;
}

void UCB_HealAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	BaseCharacter = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());
	UAbilityTask_PlayMontageAndWait* PlayHealTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, HealMontage, 1.f);
	PlayHealTask->OnCompleted.AddUniqueDynamic(this, &UCB_HealAbility::OnCompleteCallback);
	PlayHealTask->OnBlendOut.AddUniqueDynamic(this, &UCB_HealAbility::OnCancelCallback);
	PlayHealTask->OnCancelled.AddUniqueDynamic(this, &UCB_HealAbility::OnCancelCallback);
	PlayHealTask->OnInterrupted.AddUniqueDynamic(this, &UCB_HealAbility::OnCancelCallback);
	PlayHealTask->ReadyForActivation();
}

void UCB_HealAbility::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCB_HealAbility::OnCancelCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
