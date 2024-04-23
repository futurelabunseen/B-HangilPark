// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/CB_AttackHitCheckAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/AbilityTasks/CB_TraceTask.h"
#include "Abilities/TargetActor/CB_TraceTargetActor.h"
#include "Attribute/CB_CharacterAttributeSet.h"
#include "Character/CB_BaseCharacter.h"

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
		UE_LOG(LogTemp, Warning, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());

		ACB_BaseCharacter* Character = CastChecked<ACB_BaseCharacter>(HitResult.GetActor());
		Character->PlayMontage();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
			
			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddHitResult(HitResult);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;
			CueParam.Location = HitResult.ImpactPoint;		// ImpactPoint에서 호출
			TargetASC->ExecuteGameplayCue(GAMEPLAYCUE_ATTACKHIT, CueParam);
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
