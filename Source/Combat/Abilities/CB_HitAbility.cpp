// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_HitAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/CB_BaseCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Tags/StateTag.h"
#include "Interface/CB_CameraShakeInterface.h"

#include "AbilitySystemComponent.h"

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
	bIsBlocking = false;

	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TriggerEventData->TargetData, 0);
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(HitEffect);

	FName SectionName = CheckSectionName(TriggerEventData->EventMagnitude);
	if (ParryingCheck(SectionName))
		DoTargetActorStun(CastChecked<ACB_BaseCharacter>(TriggerEventData->OptionalObject));
	else
		ApplyGameplayEffect(EffectSpecHandle);

	PlayGameplayCue(EffectSpecHandle, HitResult);
	DoCameraShake();
	PlayMontgage(SectionName);
	
}

void UCB_HitAbility::ApplyGameplayEffect(const FGameplayEffectSpecHandle& EffectSpecHandle)
{
	if (EffectSpecHandle.IsValid())
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
}

void UCB_HitAbility::PlayGameplayCue(const FGameplayEffectSpecHandle& EffectSpecHandle, const FHitResult& HitResult)
{
	if (EffectSpecHandle.IsValid())
	{
		FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
		CueContextHandle.AddHitResult(HitResult);
		FGameplayCueParameters CueParam;
		CueParam.EffectContext = CueContextHandle;
	
		FGameplayTag Tag = (bIsBlocking) ? GAMEPLAYCUE_BLOCKHIT: GAMEPLAYCUE_ATTACKHIT;
		BaseCharacter->GetAbilitySystemComponent()->ExecuteGameplayCue(Tag, CueParam);
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

void UCB_HitAbility::DoCameraShake()
{
	ICB_CameraShakeInterface* Interface = Cast<ICB_CameraShakeInterface>(BaseCharacter->GetController());
	if (Interface)
		Interface->DoCameraShake();
}

FName UCB_HitAbility::CheckSectionName(const float Theta)
{
	static const FHitData Sections[] = {
		{ MakeTuple(-45, 45), FName("Fwd")},
		{ MakeTuple(45, 135), FName("Right")},
		{ MakeTuple(-135, -45), FName("Left")}
	};

	for (const auto& Section : Sections)
	{
		if (Section.Range.Key <= Theta && Theta < Section.Range.Value)
		{
			if (BaseCharacter->HasGameplayTag(STATE_GUARD))
			{
				bIsBlocking = true;
				return FName("BlockHit");
			}
			else
				return Section.Name;
		}
	}
	return FName("Bwd");
}

bool UCB_HitAbility::ParryingCheck(const FName& SectionName)
{
	if (SectionName == FName("BlockHit") && BaseCharacter->HasGameplayTag(STATE_PARRY))
		return true;
	else
		return false;
}

void UCB_HitAbility::DoTargetActorStun(ACB_BaseCharacter* Target)
{
	GetWorld()->GetWorldSettings()->SetTimeDilation(0.3f);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
		GetWorld()->GetWorldSettings()->SetTimeDilation(1.f);
		}), .15f, false);

	const FGameplayEffectSpecHandle ParryEffectSpecHandle = Target->GetAbilitySystemComponent()->
		MakeOutgoingSpec(ParryEffect, 1.f, BaseCharacter->GetAbilitySystemComponent()->MakeEffectContext());
	if (ParryEffectSpecHandle.IsValid())
		Target->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*ParryEffectSpecHandle.Data.Get());

	UAnimInstance* AnimInstance = Target->GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(ParryMontage, 1.0f);
}

void UCB_HitAbility::PlayMontgage(const FName& SectionName)
{
	UAbilityTask_PlayMontageAndWait* PlayHitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, HitMontage, 1.f, SectionName);
	PlayHitTask->OnCompleted.AddUniqueDynamic(this, &UCB_HitAbility::OnCompleteCallback);
	PlayHitTask->OnBlendOut.AddUniqueDynamic(this, &UCB_HitAbility::OnCancelCallback);
	PlayHitTask->OnCancelled.AddUniqueDynamic(this, &UCB_HitAbility::OnCancelCallback);
	PlayHitTask->OnInterrupted.AddUniqueDynamic(this, &UCB_HitAbility::OnCancelCallback);
	PlayHitTask->ReadyForActivation();
}
