// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_HitAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/CB_BaseCharacter.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Tags/StateTag.h"
#include "Kismet/KismetMathLibrary.h"

UCB_HitAbility::UCB_HitAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;

}

void UCB_HitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	BaseCharacter = CastChecked<ACB_BaseCharacter>(ActorInfo->AvatarActor.Get());

	PlayGameplayCue(TriggerEventData);

	FName SectionName = CheckSectionName(CheckTheta(TriggerEventData->TargetData));
	UAbilityTask_PlayMontageAndWait* PlayHitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, HitMontage, 1.f, SectionName);
	PlayHitTask->OnCompleted.AddUniqueDynamic(this, &UCB_HitAbility::OnCompleteCallback);
	PlayHitTask->OnBlendOut.AddUniqueDynamic(this, &UCB_HitAbility::OnCancelCallback);
	PlayHitTask->OnCancelled.AddUniqueDynamic(this, &UCB_HitAbility::OnCancelCallback);
	PlayHitTask->OnInterrupted.AddUniqueDynamic(this, &UCB_HitAbility::OnCancelCallback);
	PlayHitTask->ReadyForActivation(); 
	
}

void UCB_HitAbility::PlayGameplayCue(const FGameplayEventData* TriggerEventData)
{
	const FGameplayAbilityTargetDataHandle& TargetDataHandle = TriggerEventData->TargetData;
	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(HitEffect);
	if (EffectSpecHandle.IsValid())
	{
		ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo,
			CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

		FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::
			GetEffectContext(EffectSpecHandle);
		CueContextHandle.AddHitResult(HitResult);
		FGameplayCueParameters CueParam;
		CueParam.EffectContext = CueContextHandle;

		BaseCharacter->GetAbilitySystemComponent()->ExecuteGameplayCue(GAMEPLAYCUE_ATTACKHIT, CueParam);
	}
}

void UCB_HitAbility::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCB_HitAbility::OnCancelCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

float UCB_HitAbility::CheckTheta(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	const FVector Forward = BaseCharacter->GetActorForwardVector();

	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
	FVector Start = BaseCharacter->GetActorLocation();
	Start.Z = 0.f;
	const FVector ImpactLowered(HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y, Start.Z);
	const FVector ToHit = (ImpactLowered - Start).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	
	return UKismetMathLibrary::DegAcos(CosTheta);
}
