// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/CB_Sword.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACB_Sword::ACB_Sword()
{
	PrimaryActorTick.bCanEverTick = false;
	
	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticle"));
	TrailParticle->AttachToComponent(GetWeaponMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	TrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(GetRootComponent());

}

void ACB_Sword::BeginPlay()
{
	Super::BeginPlay();
	const FVector BotPos = GetWeaponMesh()->GetSocketLocation("StartSocket");
	const FVector TopPos = GetWeaponMesh()->GetSocketLocation("EndSocket");

	TrailEffect->SetFloatParameter("RibbonWidth", (TopPos - BotPos).Length());
	TrailEffect->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(BotPos - TopPos));
	TrailEffect->SetWorldLocation((TopPos + BotPos) / 2);
	TrailEffect->Deactivate();

}

void ACB_Sword::TrailStart()
{
	TrailParticle->BeginTrails("StartSocket", "EndSocket", ETrailWidthMode_FromCentre, 1.f);
	TrailEffect->Activate();
}

void ACB_Sword::TrailEnd()
{
	TrailParticle->EndTrails();
	TrailEffect->Deactivate();
}
