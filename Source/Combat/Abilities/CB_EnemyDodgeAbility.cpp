// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_EnemyDodgeAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UCB_EnemyDodgeAbility::UCB_EnemyDodgeAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCB_EnemyDodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);


	UAbilityTask_PlayMontageAndWait* PlayDodgeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, DodgeMontage, 1.f, RandomSection());
	PlayDodgeTask->OnCompleted.AddUniqueDynamic(this, &UCB_EnemyDodgeAbility::OnCompleteCallback);
	PlayDodgeTask->OnBlendOut.AddUniqueDynamic(this, &UCB_EnemyDodgeAbility::OnCompleteCallback);
	PlayDodgeTask->OnInterrupted.AddUniqueDynamic(this, &UCB_EnemyDodgeAbility::OnCompleteCallback);
	PlayDodgeTask->OnCancelled.AddUniqueDynamic(this, &UCB_EnemyDodgeAbility::OnCompleteCallback);
	PlayDodgeTask->ReadyForActivation();
}

void UCB_EnemyDodgeAbility::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

FName UCB_EnemyDodgeAbility::RandomSection()
{
	const int32 MaxIdx = DodgeMontageSections.Num() - 1;
	int32 RandIdx = FMath::RandRange(0, MaxIdx);

	return DodgeMontageSections[RandIdx];
}
