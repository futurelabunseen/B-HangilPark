// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/ANS/CB_ANS_WeaponTrail.h"
#include "Character/CB_BaseCharacter.h"
#include "Weapon/CB_Sword.h"

void UCB_ANS_WeaponTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	Character = Cast<ACB_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Sword = Cast<ACB_Sword>(Character->GetWeapon());
		Sword->TrailStart();
	}
}

void UCB_ANS_WeaponTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (Sword)
		Sword->TrailEnd();
}
