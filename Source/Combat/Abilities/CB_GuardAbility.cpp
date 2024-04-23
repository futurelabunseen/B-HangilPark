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
	const bool bIsGaurd = BaseCharacter->GetIsGuard();
	const bool bHasTag = BaseCharacter->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(TagContainer);
	// ��Ŭ�� O, �±� O, -
	// ��Ŭ�� O, �±� X, add
	// ��Ŭ�� X, �±� O, remove
	// ��Ŭ�� X, �±� X, remove

	if (bIsGaurd && !bHasTag)
		UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(BaseCharacter, TagContainer);
	else
	{
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(BaseCharacter, TagContainer);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UCB_GuardAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(BaseCharacter, TagContainer);
}
