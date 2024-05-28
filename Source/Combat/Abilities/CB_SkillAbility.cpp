// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_SkillAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/CB_BaseCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UCB_SkillAbility::UCB_SkillAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;
}

void UCB_SkillAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	BaseCharacter = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());
	UAbilityTask_PlayMontageAndWait* PlaySkillTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, SkillMontage, 1.f);
	PlaySkillTask->OnCompleted.AddUniqueDynamic(this, &UCB_SkillAbility::OnCompleteCallback);
	PlaySkillTask->OnBlendOut.AddUniqueDynamic(this, &UCB_SkillAbility::OnCancelCallback);
	PlaySkillTask->OnCancelled.AddUniqueDynamic(this, &UCB_SkillAbility::OnCancelCallback);
	PlaySkillTask->OnInterrupted.AddUniqueDynamic(this, &UCB_SkillAbility::OnCancelCallback);
	PlaySkillTask->ReadyForActivation();
}

void UCB_SkillAbility::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCB_SkillAbility::OnCancelCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
