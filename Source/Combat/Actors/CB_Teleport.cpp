// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_Teleport.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "Gamemode/CB_GameMode.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ACB_Teleport::ACB_Teleport()
{
	PrimaryActorTick.bCanEverTick = false;

	TeleportMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportMesh"));
	TeleportMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TeleportMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(TeleportMesh);

	TeleportArea = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TeleportArea"));
	TeleportArea->SetupAttachment(GetRootComponent());

	TeleportEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TeleportEffect"));
	TeleportEffect->SetupAttachment(GetRootComponent());
}

void ACB_Teleport::BeginPlay()
{
	Super::BeginPlay();

	TeleportArea->OnComponentBeginOverlap.AddDynamic(this, &ACB_Teleport::OnCapsuleOverlap);

}

void ACB_Teleport::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ACharacter* Character = Cast<ACharacter>(OtherActor);
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		ACB_GameMode* GameMode = Cast<ACB_GameMode>(GetWorld()->GetAuthGameMode());
		GameMode->AsyncLevelLoad("/Game/Maps/", LevelName);
	}
}
