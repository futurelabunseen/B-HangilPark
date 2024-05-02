// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_DeadAbility.h"
#include "Character/CB_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Tags/StateTag.h"

UCB_DeadAbility::UCB_DeadAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCB_DeadAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	BaseCharacter = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());
	BaseCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	BaseCharacter->SetActorEnableCollision(false);

	// 델리게이트를 통해 LockOn 및 HUD 해제

	FGameplayEventData Data;
	UAbilityTask_WaitGameplayEvent* WaitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
		STATE_HIT, nullptr, true, true);
	WaitEvent->EventReceived.AddUniqueDynamic(this, &UCB_DeadAbility::PlayMontage);
	WaitEvent->ReadyForActivation();
}

void UCB_DeadAbility::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCB_DeadAbility::PlayMontage(FGameplayEventData Data)
{
	UAbilityTask_PlayMontageAndWait* PlayHitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DeadMontage);
	PlayHitTask->OnCompleted.AddUniqueDynamic(this, &UCB_DeadAbility::OnCompleteCallback);
	PlayHitTask->ReadyForActivation();
}
