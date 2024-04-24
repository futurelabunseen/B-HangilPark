// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CB_DeathAbility.h"
#include "Character/CB_BaseCharacter.h"

UCB_DeathAbility::UCB_DeathAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCB_DeathAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	UE_LOG(LogTemp, Warning, TEXT("DeathAbility Entry"));

	BaseCharacter = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());
	// 이동 못하게 설정

	UAnimInstance* AnimInstance = BaseCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
		AnimInstance->Montage_Play(DeathMontage);

}

void UCB_DeathAbility::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
