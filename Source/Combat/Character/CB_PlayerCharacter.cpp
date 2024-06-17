// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "State/CB_PlayerState.h"
#include "Controller/CB_PlayerController.h"
#include "UI/HUD/CB_HUD.h"
#include "Attribute/CB_CharacterAttributeSet.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CB_TrailComponent.h"
#include "Components/CB_LockOnComponent.h"
#include "MotionWarpingComponent.h"
#include "Gamemode/CB_GameMode.h"
#include "Blueprint/UserWidget.h"

ACB_PlayerCharacter::ACB_PlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	TrailComponent = CreateDefaultSubobject<UCB_TrailComponent>(TEXT("TrailComponent"));
}

void ACB_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ACB_PlayerState* PS = GetPlayerState<ACB_PlayerState>();
	if (PS)
	{
		ASC = PS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(PS, this);

		for (const auto& Ability : Abilities)
		{
			FGameplayAbilitySpec Spec(Ability);
			ASC->GiveAbility(Spec);
		}
		
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		for (TSubclassOf<UGameplayEffect> GameplayEffect : Effects)
		{
			FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(GameplayEffect, 1.f, EffectContext);
			if (NewHandle.IsValid())
				FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(
					*NewHandle.Data.Get(), ASC.Get());
		}

		ACB_PlayerController* PC = CastChecked<ACB_PlayerController>(NewController);
		//PC->ConsoleCommand(TEXT("showdebug abilitysystem"));
		PC->LockChangeDelegate.BindUFunction(this, FName("LockChange"));

		UCB_CharacterAttributeSet* AS = CastChecked<UCB_CharacterAttributeSet>(PS->GetAttributeSet());
		// 나중에 컨트롤러에서 관리하도록 설정
		if (ACB_HUD* Hud = Cast<ACB_HUD>(PC->GetHUD()))
			Hud->InitOverlay(PC, PS, ASC, AS);
	}
}

void ACB_PlayerCharacter::TrailStart(FGameplayTag Tag)
{
	TrailComponent->StartTrail(Tag);
}

void ACB_PlayerCharacter::SetWarpTarget()
{
	Super::SetWarpTarget();
	if (IsLocked())
	{
		FTransform Transform = LockOnComponent->GetLockedOnTargetActor()->GetActorTransform();
		MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(TEXT("Target"), Transform);
	}
	else
	{
		MotionWarpingComponent->RemoveWarpTarget(TEXT("Target"));
	}
}

void ACB_PlayerCharacter::Dead()
{
	ACB_PlayerController* PC = CastChecked<ACB_PlayerController>(GetController());
	PC->SetPlayerInputMode(true);

	if (IsValid(DiedOverlayClass))
	{
		DiedOverlay = CreateWidget(GetWorld(), DiedOverlayClass);
		if (IsValid(DiedOverlay))
			DiedOverlay->AddToViewport();
	}
}
