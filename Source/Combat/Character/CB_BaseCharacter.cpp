// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Components/CB_LockOnComponent.h"
#include "../Tags/StateTag.h"
#include "../Weapon/CB_BaseWeapon.h"

ACB_BaseCharacter::ACB_BaseCharacter()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.f), FRotator(0, -90.0f, 0));
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 450.f;

	LockOnComponent = CreateDefaultSubobject<UCB_LockOnComponent>(TEXT("LockOnComponent"));
}

void ACB_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<ACB_BaseWeapon>(WeaponClass);
		Weapon->Equip(GetMesh(), FName("HolsterSocket"), this, this);
	}
}

void ACB_BaseCharacter::LockOn()
{
	LockOnComponent->TargetActor();
}

bool ACB_BaseCharacter::IsLocked()
{
	return LockOnComponent->IsLocked();
}

void ACB_BaseCharacter::LockChange(float Axis)
{
	if (!HasGameplayTag(STATE_DODGE))
		LockOnComponent->TargetActorWithAxisInput(Axis);
}

bool ACB_BaseCharacter::HasGameplayTag(FGameplayTag Tag) const
{
	if (IsValid(ASC))
		return ASC->HasMatchingGameplayTag(Tag);
	else
		return false;
}
