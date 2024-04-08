// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_ANS_Ready.h"
#include "AbilitySystemBlueprintLibrary.h"

void UCB_ANS_Ready::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(MeshComp->GetOwner(), AttackBeginTags);
}

void UCB_ANS_Ready::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(MeshComp->GetOwner(), AttackBeginTags);
}
