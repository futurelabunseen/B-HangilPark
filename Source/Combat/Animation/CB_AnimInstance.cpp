// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCB_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<ACharacter>(TryGetPawnOwner());
	if (Character)
		CharacterMovement = Character->GetCharacterMovement();
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
	}
}
