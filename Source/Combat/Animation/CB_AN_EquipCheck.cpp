// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_AN_EquipCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../Character/CB_BaseCharacter.h"
#include "../Weapon/CB_BaseWeapon.h"

void UCB_AN_EquipCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	ACB_BaseCharacter* Character = Cast<ACB_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
		Character->GetWeapon()->AttachMeshToSocket(Character->GetMesh(), SocketName);
}
