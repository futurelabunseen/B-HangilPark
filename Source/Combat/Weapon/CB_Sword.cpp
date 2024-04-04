// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CB_Sword.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"


ACB_Sword::ACB_Sword()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetRootComponent());
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticle"));
	TrailParticle->AttachToComponent(GetWeaponMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

void ACB_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FColor DrawColor = (bCollisionOn) ? FColor::Green : FColor::Red;
	if (GetWorld())
	{
		DrawDebugLine(GetWorld(), GetWeaponMesh()->GetSocketLocation("StartSocket"), 
			GetWeaponMesh()->GetSocketLocation("EndSocket"), DrawColor, false, -1, 0, 5.f);

	}
}

void ACB_Sword::CollisionOn()
{
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	bCollisionOn = true;
}

void ACB_Sword::CollisionOff()
{
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bCollisionOn = false;
}

void ACB_Sword::TrailStart()
{
	if (TrailParticle)
		TrailParticle->BeginTrails("StartSocket", "EndSocket", ETrailWidthMode_FromCentre, 1.f);
}

void ACB_Sword::TrailEnd()
{
	if (TrailParticle)
		TrailParticle->EndTrails();
}
