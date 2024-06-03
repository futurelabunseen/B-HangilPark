// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CB_LockOnComponent.h"
#include "Tags/StateTag.h"
#include "Weapon/CB_BaseWeapon.h"
#include "Data/CB_DA_Weapon.h"
#include "MotionWarpingComponent.h"

ACB_BaseCharacter::ACB_BaseCharacter()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.f), FRotator(0, -90.0f, 0));
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed::Run;

	LockOnComponent = CreateDefaultSubobject<UCB_LockOnComponent>(TEXT("LockOnComponent"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void ACB_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponData->WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<ACB_BaseWeapon>(WeaponData->WeaponClass);
		Weapon->Equip(GetMesh(), FName("HolsterSocket"), this, this);
	}
}

void ACB_BaseCharacter::SetIsGuard(const bool IsGaurd)
{
	bIsGuard = IsGaurd;
	GetCharacterMovement()->MaxWalkSpeed = (IsGaurd) ? WalkSpeed::Walk : WalkSpeed::Run;
}

void ACB_BaseCharacter::LockOn()
{
	if (IsValid(LockOnComponent))
		LockOnComponent->TargetActor();
}

bool ACB_BaseCharacter::IsLocked()
{
	if (IsValid(LockOnComponent))
		return LockOnComponent->IsLocked();
	return false;
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

void ACB_BaseCharacter::AddUniqueGameplayTag(FGameplayTag Tag)
{
	if (!HasGameplayTag(Tag))
		ASC->AddLooseGameplayTag(Tag);
}

void ACB_BaseCharacter::RemoveUniqueGameplayTag(FGameplayTag Tag)
{
	if (HasGameplayTag(Tag))
		ASC->RemoveLooseGameplayTag(Tag);
}

FVector ACB_BaseCharacter::GetSocketLocation(const FName SocketName)
{
	return GetMesh()->GetSocketLocation(SocketName);
}

FVector ACB_BaseCharacter::GetWeaponSocketLocation(const FName SocketName)
{
	return Weapon->GetWeaponMesh()->GetSocketLocation(SocketName);
}

void ACB_BaseCharacter::Dead()
{
	const static float DeadEventDelayTime = 2.0f;

	if (GetController())
		GetController()->PawnPendingDestroy(this);
		//GetController()->UnPossess();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
		ASC->CancelAllAbilities();
		ASC->ClearAllAbilities();

		
		if (GetWeapon())
			GetWeapon()->Destroy();

		Destroy();
		}), DeadEventDelayTime, false);
}
