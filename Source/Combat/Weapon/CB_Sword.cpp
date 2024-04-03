// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CB_Sword.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACB_Sword::ACB_Sword()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetRootComponent());
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticle"));
	TrailParticle->AttachToComponent(GetWeaponMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

void ACB_Sword::TrailStart()
{
	TrailParticle->BeginTrails("StartSocket", "EndSocket", ETrailWidthMode_FromCentre, 1.f);
}

void ACB_Sword::TrailEnd()
{
	TrailParticle->EndTrails();
}
