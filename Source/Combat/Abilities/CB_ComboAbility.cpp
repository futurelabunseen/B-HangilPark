// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_ComboAbility.h"
#include "Character/CB_BaseCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Tags/StateTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Data/CB_DA_Weapon.h"

UCB_ComboAbility::UCB_ComboAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(STATE_ATTACK_LIGHT);
	ActivationBlockedTags.AddTag(STATE_DODGE);
	ActivationBlockedTags.AddTag(STATE_JUMP);
}

bool UCB_ComboAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bResult)
		return false;

	const ACB_BaseCharacter* Character = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());
	bool bIsEquipOn = Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(STATE_EQUIPMENT_ON);
	if (!bIsEquipOn)
	{
		FGameplayEventData Payload;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			ActorInfo->AvatarActor.Get(), STATE_EQUIPMENT, Payload);
	}
	return bIsEquipOn;
}

void UCB_ComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	BaseCharacter = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());
	FGameplayEventData Data;
	if (!IsFirstAttack)
	{
		UAbilityTask_WaitGameplayEvent* WaitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
			STATE_ATTACK_LIGHT_NEXT, nullptr, true, true);
		WaitEvent->EventReceived.AddUniqueDynamic(this, &UCB_ComboAbility::PlayMontage);
		WaitEvent->ReadyForActivation();	
	}
	else
	{
		PlayMontage(Data);
	}
}

void UCB_ComboAbility::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCB_ComboAbility::OnCancelCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UCB_ComboAbility::PlayMontage(FGameplayEventData Data)
{
	if (!BaseCharacter->IsLocked())
		ImmediateRotateActor();

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, BaseCharacter->GetWeaponData()->ComboAttackMontage, 1.f, SectionName);
		
	PlayAttackTask->OnCompleted.AddUniqueDynamic(this, &UCB_ComboAbility::OnCompleteCallback);
	PlayAttackTask->OnBlendOut.AddUniqueDynamic(this, &UCB_ComboAbility::OnCancelCallback);
	PlayAttackTask->OnInterrupted.AddUniqueDynamic(this, &UCB_ComboAbility::OnCancelCallback);
	PlayAttackTask->OnCancelled.AddUniqueDynamic(this, &UCB_ComboAbility::OnCancelCallback);
	PlayAttackTask->ReadyForActivation();
}

void UCB_ComboAbility::ImmediateRotateActor()
{
	if (BaseCharacter->GetLastMovementInputVector().Size() > 0.f)
	{
		FRotator Rotate = BaseCharacter->GetLastMovementInputVector().Rotation();
		BaseCharacter->SetActorRotation(Rotate.Quaternion());
	}
}
