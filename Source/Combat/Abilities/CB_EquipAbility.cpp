// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_EquipAbility.h"
#include "../Character/CB_BaseCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Tags/StateTag.h"

UCB_EquipAbility::UCB_EquipAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
}

void UCB_EquipAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	BaseCharacter = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());
	UAbilityTask_PlayMontageAndWait* PlayDodgeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, EquipMontage,1.f, GetSectionName());
	PlayDodgeTask->OnCompleted.AddDynamic(this, &UCB_EquipAbility::OnCompleteCallback);
	PlayDodgeTask->OnInterrupted.AddDynamic(this, &UCB_EquipAbility::OnCompleteCallback);
	PlayDodgeTask->ReadyForActivation();
}


FName UCB_EquipAbility::GetSectionName()
{
	return (BaseCharacter->GetIsEquipped()) ? FName("Off") : FName("On");
}

void UCB_EquipAbility::OnCompleteCallback()
{
	BaseCharacter->FlipIsEquipped();

	if (BaseCharacter->GetIsEquipped())
		BaseCharacter->AddGameplayTag(STATE_EQUIPMENT_ON);
	else
		BaseCharacter->RemoveGameplayTag(STATE_EQUIPMENT_ON);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
