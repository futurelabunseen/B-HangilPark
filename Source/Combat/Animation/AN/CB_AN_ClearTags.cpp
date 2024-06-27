// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_AN_ClearTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UCB_AN_ClearTags::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(MeshComp->GetOwner(), RemoveTags);
}
