// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_DeadAbility.h"
#include "Character/CB_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	UAnimInstance* AnimInstance = BaseCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
