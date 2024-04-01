// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_ANS_Branch.h"
#include "AbilitySystemBlueprintLibrary.h"


#define STATE_ATTACK_LIGHT_NEXT FGameplayTag::RequestGameplayTag(FName("State.Attack.Light.Next"))


void UCB_ANS_Branch::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	FGameplayEventData Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		MeshComp->GetOwner(), STATE_ATTACK_LIGHT_NEXT, Payload);
}

