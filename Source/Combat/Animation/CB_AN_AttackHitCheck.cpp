// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/CB_AN_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

void UCB_AN_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggetGameplayTag, PayloadData);
		}
	}
}
