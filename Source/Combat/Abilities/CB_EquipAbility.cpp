// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_EquipAbility.h"
#include "Character/CB_BaseCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"

UCB_EquipAbility::UCB_EquipAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCB_EquipAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	BaseCharacter = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());
	SetValue();

	UAbilityTask_PlayMontageAndWait* PlayEquipTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, EquipMontage,1.f, SectionName);
	PlayEquipTask->OnCompleted.AddUniqueDynamic(this, &UCB_EquipAbility::OnCompleteCallback);
	PlayEquipTask->OnInterrupted.AddUniqueDynamic(this, &UCB_EquipAbility::OnCompleteCallback);
	PlayEquipTask->ReadyForActivation();
}

void UCB_EquipAbility::SetValue()
{
	if (BaseCharacter->HasGameplayTag(STATE_EQUIPMENT_ON))
	{
		BaseCharacter->RemoveGameplayTag(STATE_EQUIPMENT_ON);
		SectionName = FName("Off");
	}
	else
	{
		BaseCharacter->AddGameplayTag(STATE_EQUIPMENT_ON);
		SectionName= FName("On");
	}
}

void UCB_EquipAbility::OnCompleteCallback()
{	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
