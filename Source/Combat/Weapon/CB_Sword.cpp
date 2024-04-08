// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/CB_Sword.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"

ACB_Sword::ACB_Sword()
{
	PrimaryActorTick.bCanEverTick = true;

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticle"));
	TrailParticle->AttachToComponent(GetWeaponMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

void ACB_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
