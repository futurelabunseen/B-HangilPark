// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_AnimInstance.h"
#include "../Character/CB_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCB_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// Character���� BaseCharacter�� ��üȭ��
	Character = Cast<ACB_BaseCharacter>(TryGetPawnOwner());
	if (Character)
	{
		CharacterMovement = Character->GetCharacterMovement();
	}
}

void UCB_AnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Character && CharacterMovement)
	{
		Velocity = CharacterMovement->Velocity;
		bIsFalling = CharacterMovement->IsFalling();
		bShouldMove = (CharacterMovement->GetCurrentAcceleration() != FVector(0, 0, 0));
		MoveForward = FVector::DotProduct(Character->GetActorForwardVector(), Velocity);
		MoveRight = FVector::DotProduct(Character->GetActorRightVector(), Velocity);
		// ������?
		IsEquip = Character->HasGameplayTag(STATE_EQUIPMENT_ON);
	}
}
