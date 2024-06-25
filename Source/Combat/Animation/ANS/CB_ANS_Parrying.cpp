// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_ANS_Parrying.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Data/StateTag.h"

void UCB_ANS_Parrying::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	FGameplayEventData Payload;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (IsValid(ASC))
	{
		bool bDuringGuard = ASC->HasMatchingGameplayTag(STATE_GUARD);
		if (!bDuringGuard)
			UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(MeshComp->GetOwner(), ParryTags);
	}
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), STATE_GUARD, Payload);
}

void UCB_ANS_Parrying::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(MeshComp->GetOwner(), ParryTags);
}
