// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_ANS_Trail.h"
#include "Character/CB_BaseCharacter.h"
#include "Weapon/CB_Sword.h"

// 수정 필요
void UCB_ANS_Trail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ACB_BaseCharacter* Character = Cast<ACB_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		ACB_Sword* Sword = Cast<ACB_Sword>(Character->GetWeapon());
		Sword->TrailStart();
	}
}

void UCB_ANS_Trail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	ACB_BaseCharacter* Character = Cast<ACB_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		ACB_Sword* Sword = Cast<ACB_Sword>(Character->GetWeapon());
		Sword->TrailEnd();
	}
}
