// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_GuardAbility.h"
#include "Character/CB_BaseCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tags/StateTag.h"

UCB_GuardAbility::UCB_GuardAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCB_GuardAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	BaseCharacter = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());
	
	if(BaseCharacter->GetIsGuard())
	{
		BaseCharacter->AddUniqueGameplayTag(STATE_GUARD);
	}
	else
	{
		BaseCharacter->RemoveUniqueGameplayTag(STATE_GUARD);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UCB_GuardAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	BaseCharacter->RemoveUniqueGameplayTag(STATE_GUARD);

}
