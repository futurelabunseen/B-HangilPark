// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_ANS_Branch.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Data/StateTag.h"

void UCB_ANS_Branch::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	FGameplayEventData Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), STATE_ATTACK_LIGHT_NEXT, Payload);
}

