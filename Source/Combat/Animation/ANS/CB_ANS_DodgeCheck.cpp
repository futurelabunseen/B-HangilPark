// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_ANS_DodgeCheck.h"

void UCB_ANS_DodgeCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UCB_ANS_DodgeCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}