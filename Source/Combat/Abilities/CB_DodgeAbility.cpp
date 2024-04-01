// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_DodgeAbility.h"
#include "../Character/CB_BaseCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"

UCB_DodgeAbility::UCB_DodgeAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCB_DodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	BaseCharacter = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());

	ImmediateRotateActor();
	UAbilityTask_PlayMontageAndWait* PlayDodgeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, DodgeMontage);
	PlayDodgeTask->OnCompleted.AddDynamic(this, &UCB_DodgeAbility::OnCompleteCallback);
	PlayDodgeTask->ReadyForActivation();
}

void UCB_DodgeAbility::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCB_DodgeAbility::ImmediateRotateActor()
{
	if (BaseCharacter->GetLastMovementInputVector().Size() > 0.f)
	{
		FRotator Rotate = BaseCharacter->GetLastMovementInputVector().Rotation();
		BaseCharacter->SetActorRotation(Rotate.Quaternion());
	}
}
