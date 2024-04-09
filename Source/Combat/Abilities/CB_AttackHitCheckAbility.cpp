// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CB_AttackHitCheckAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/AbilityTasks/CB_TraceTask.h"
#include "Abilities/TargetActor/CB_TraceTargetActor.h"
#include "Attribute/CB_CharacterAttributeSet.h"
#include "Tags/StateTag.h"

UCB_AttackHitCheckAbility::UCB_AttackHitCheckAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCB_AttackHitCheckAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UCB_TraceTask* AttackTraceTask = UCB_TraceTask::CreateTask(this, ACB_TraceTargetActor::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UCB_AttackHitCheckAbility::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UCB_AttackHitCheckAbility::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UE_LOG(LogTemp, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
		if (EffectSpecHandle.IsValid())
		{
			// 데미지 수치 전달
			EffectSpecHandle.Data->SetSetByCallerMagnitude(DATA_DAMAGE, -20.f);
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
