// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_AN_SendEvent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UCB_AN_SendEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	FGameplayEventData Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), TriggerGameplayTag, Payload);
}