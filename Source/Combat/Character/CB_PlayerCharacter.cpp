// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "../State/CB_PlayerState.h"

ACB_PlayerCharacter::ACB_PlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

void ACB_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ACB_PlayerState* GASPS = GetPlayerState<ACB_PlayerState>();
	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);

		for (const auto& Ability : Abilities)
		{
			FGameplayAbilitySpec Spec(Ability);
			ASC->GiveAbility(Spec);
		}

		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

void ACB_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ACB_PlayerCharacter::InputPressed(const FGameplayTagContainer& GameplayTagContainer)
{
	ASC->TryActivateAbilitiesByTag(GameplayTagContainer);
}

void ACB_PlayerCharacter::InputReleased(const FGameplayTagContainer& GameplayTagContainer)
{
}
