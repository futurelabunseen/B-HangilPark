// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_PlayerController.h"
#include "Data/CB_PDA_Input.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Character/CB_PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "Camera/CB_LegacyCameraShake.h"
#include "UI/HUD/CB_HUD.h"
#include "UI/Widgets/CB_UserWidget.h"

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

	EIC->BindAction(InputData->GuardAction, ETriggerEvent::Started, this,
		&ACB_PlayerController::Guard, true);
	EIC->BindAction(InputData->GuardAction, ETriggerEvent::Completed, this,
		&ACB_PlayerController::Guard, false);

	EIC->BindAction(InputData->HealAction, ETriggerEvent::Triggered, this,
		&ACB_PlayerController::InputPressed, STATE_HEAL);

	EIC->BindAction(InputData->QSkillAction, ETriggerEvent::Triggered, this,
		&ACB_PlayerController::InputPressed, STATE_ATTACK_SKILL_Q);

	EIC->BindAction(InputData->ESkillAction, ETriggerEvent::Triggered, this,
		&ACB_PlayerController::InputPressed, STATE_ATTACK_SKILL_E);
}

void ACB_PlayerController::Move(const FInputActionValue& Value)
{
	if (!IsValid(GetCharacter()))
		return;

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
	if (!IsValid(GetCharacter()))
		return;

	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	GetCharacter()->AddControllerYawInput(LookAxisVector.X);
	GetCharacter()->AddControllerPitchInput(LookAxisVector.Y);

	LockChangeDelegate.Execute(LookAxisVector.X);
}

void ACB_PlayerController::LockOn()
{
	if (!IsValid(GetCharacter()))
		return;
	ACB_PlayerCharacter* PlayerCharacter = Cast<ACB_PlayerCharacter>(GetCharacter());
	PlayerCharacter->LockOn();
}

void ACB_PlayerController::Guard(bool bIsActiave)
{
	if (!IsValid(GetCharacter()))
		return;
	ACB_PlayerCharacter* PlayerCharacter = Cast<ACB_PlayerCharacter>(GetCharacter());
	PlayerCharacter->SetIsGuard(bIsActiave);

	if (!bIsActiave)
		PlayerCharacter->RemoveUniqueGameplayTag(STATE_GUARD);

}

void ACB_PlayerController::InputPressed(const FGameplayTag Tag)
{
	if (!IsValid(GetCharacter()))
		return;

	FGameplayTagContainer Container;
	Container.AddTag(Tag);

	ACB_PlayerCharacter* PlayerCharacter = Cast<ACB_PlayerCharacter>(GetCharacter());
	UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();

	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(Container, AbilitiesToActivate);
	for (auto GameplayAbilitySpec : AbilitiesToActivate)
	{
		if (GameplayAbilitySpec->IsActive())
			ASC->AbilitySpecInputPressed(*GameplayAbilitySpec);
		else
			ASC->TryActivateAbility(GameplayAbilitySpec->Handle);
	}
}

void ACB_PlayerController::InputReleased(const FGameplayTag Tag)
{
	if (!IsValid(GetCharacter()))
		return;

	FGameplayTagContainer Container;
	Container.AddTag(Tag);
	ACB_PlayerCharacter* PlayerCharacter = Cast<ACB_PlayerCharacter>(GetCharacter());
	UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();

	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(Container, AbilitiesToActivate);
	for (auto GameplayAbilitySpec : AbilitiesToActivate)
	{
		if (GameplayAbilitySpec->IsActive())
			ASC->AbilitySpecInputReleased(*GameplayAbilitySpec);
	}
}

void ACB_PlayerController::DoCameraShake()
{
	if (!IsValid(GetCharacter()))
		return;

	if (IsValid(CameraShake))
		PlayerCameraManager->StartCameraShake(CameraShake, 1.f);
}

void ACB_PlayerController::SetPlayerInputMode(bool bInputMode)
{
	FInputModeDataBase* InputMode = (bInputMode)
		? static_cast<FInputModeDataBase*>(new FInputModeUIOnly())
		: static_cast<FInputModeDataBase*>(new FInputModeGameOnly());

	SetInputMode(*InputMode);
	bShowMouseCursor = bInputMode;
}

void ACB_PlayerController::ShowOverlay(bool bIsShow)
{
	ACB_HUD* Hud = Cast<ACB_HUD>(GetHUD());
	if (bIsShow)
		Hud->OverlayWidget->AddToViewport();
	else
		Hud->OverlayWidget->RemoveFromParent();
	
}
