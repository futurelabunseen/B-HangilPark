// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_ANS_AttackCheck.h"
#include "../Character/CB_BaseCharacter.h"
#include "../Weapon/CB_Sword.h"

void UCB_ANS_AttackCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ACB_BaseCharacter* Character = Cast<ACB_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		ACB_Sword* Sword = Cast<ACB_Sword>(Character->GetWeapon());
		Sword->CollisionOn();
		Sword->TrailStart();
	}
}

void UCB_ANS_AttackCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void UCB_ANS_AttackCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	// 무기 콜리전 비활성화
	ACB_BaseCharacter* Character = Cast<ACB_BaseCharacter>(MeshComp->GetOwner());
	
	if (Character)
	{
		ACB_Sword* Sword = Cast<ACB_Sword>(Character->GetWeapon());
		Sword->CollisionOff();
		Sword->TrailEnd();
	}
}
