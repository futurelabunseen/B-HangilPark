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
	BaseCharacter = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());
	SetValue();

	UAbilityTask_PlayMontageAndWait* PlayDodgeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, EquipMontage,1.f, SectionName);

	PlayDodgeTask->OnCompleted.AddDynamic(this, &UCB_EquipAbility::OnCompleteCallback);
	PlayDodgeTask->OnInterrupted.AddDynamic(this, &UCB_EquipAbility::OnCompleteCallback);
	PlayDodgeTask->ReadyForActivation();
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
