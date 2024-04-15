// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_PlayerController.h"
#include "Data/CB_PDA_Input.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Character/CB_PlayerCharacter.h"

void ACB_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputData->MappingContext, 0);
	}
}

void ACB_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent);

	EIC->BindAction(InputData->MoveAction, ETriggerEvent::Triggered, this, &ACB_PlayerController::Move);
	EIC->BindAction(InputData->LookAction, ETriggerEvent::Triggered, this, &ACB_PlayerController::Look);

	EIC->BindAction(InputData->JumpAction, ETriggerEvent::Triggered, this,
		&ACB_PlayerController::InputPressed, STATE_JUMP);
	EIC->BindAction(InputData->AttackAction, ETriggerEvent::Triggered, this,
		&ACB_PlayerController::InputPressed, STATE_ATTACK_LIGHT);
	EIC->BindAction(InputData->DodgeAction, ETriggerEvent::Triggered, this,
		&ACB_PlayerController::InputPressed, STATE_DODGE);

	EIC->BindAction(InputData->LockOnAction, ETriggerEvent::Triggered, this, &ACB_PlayerController::LockOn);
	EIC->BindAction(InputData->EquipAction, ETriggerEvent::Triggered, this,
		&ACB_PlayerController::InputPressed, STATE_EQUIPMENT);

	EIC->BindAction(InputData->GuardAction, ETriggerEvent::Started, 
		this, &ACB_PlayerController::InputPressed, STATE_GUARD);
	}

void ACB_PlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	GetCharacter()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetCharacter()->AddMovementInput(RightDirection, MovementVector.X);
}

void ACB_PlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	GetCharacter()->AddControllerYawInput(LookAxisVector.X);
	GetCharacter()->AddControllerPitchInput(LookAxisVector.Y);

	LockChangeDelegate.Execute(LookAxisVector.X);
}

void ACB_PlayerController::LockOn()
{
	ACB_PlayerCharacter* PlayerCharacter = Cast<ACB_PlayerCharacter>(GetCharacter());
	PlayerCharacter->LockOn();
}

void ACB_PlayerController::InputPressed(const FGameplayTag Tag)
{
	FGameplayTagContainer Container;
	Container.AddTag(Tag);
	ACB_PlayerCharacter* PlayerCharacter = Cast<ACB_PlayerCharacter>(GetCharacter());
	PlayerCharacter->InputPressed(Container);
}

void ACB_PlayerController::InputReleased(const FGameplayTag Tag)
{
	// ���� ���忡�� ����ҵ�?
	FGameplayTagContainer Container;
	Container.AddTag(Tag);
	ACB_PlayerCharacter* PlayerCharacter = Cast<ACB_PlayerCharacter>(GetCharacter());
	PlayerCharacter->InputReleased(Container);
}
