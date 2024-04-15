// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_GuardAbility.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

UCB_GuardAbility::UCB_GuardAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCB_GuardAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	UE_LOG(LogTemp, Warning, TEXT("Guard"));
	UAbilityTask_PlayMontageAndWait* PlayDodgeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, GuardMontage, 1.f);
	PlayDodgeTask->OnCompleted.AddDynamic(this, &UCB_GuardAbility::OnCompleteCallback);
	PlayDodgeTask->ReadyForActivation();

	//UAbilityTask_WaitInputRelease* WaitInputTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	//WaitInputTask->OnRelease.AddDynamic(this, &UCB_GuardAbility::OnCompleteCallback);

	
}

void UCB_GuardAbility::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
