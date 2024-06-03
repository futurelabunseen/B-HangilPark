// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_DeadAbility.h"
#include "Character/CB_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Tags/StateTag.h"
#include "Kismet/GameplayStatics.h"

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

	ACB_BaseCharacter* Player = Cast<ACB_BaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->LockOn();

	FGameplayEventData Data;

	UAbilityTask_WaitGameplayEvent* WaitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, STATE_HIT_LIGHT);
	WaitEvent->EventReceived.AddUniqueDynamic(this, &UCB_DeadAbility::PlayMontage);
	WaitEvent->ReadyForActivation();

	if (!BaseCharacter->HasGameplayTag(STATE_HIT_LIGHT))
		PlayMontage(Data);

}

void UCB_DeadAbility::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCB_DeadAbility::PlayMontage(FGameplayEventData Data)
{
	UAbilityTask_PlayMontageAndWait* PlayDeathTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DeadMontage);
	PlayDeathTask->OnCompleted.AddUniqueDynamic(this, &UCB_DeadAbility::OnCompleteCallback);
	PlayDeathTask->OnBlendOut.AddUniqueDynamic(this, &UCB_DeadAbility::OnCompleteCallback);
	PlayDeathTask->OnInterrupted.AddUniqueDynamic(this, &UCB_DeadAbility::OnCompleteCallback);
	PlayDeathTask->OnCancelled.AddUniqueDynamic(this, &UCB_DeadAbility::OnCompleteCallback);
	PlayDeathTask->ReadyForActivation();

	BaseCharacter->Dead();
}
