// Fill out your copyright notice in the Description page of Project Settings.

#include "Cue/CB_DotCue.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UCB_DotCue::UCB_DotCue()
{
	HitParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("HitParticle"));
}

bool UCB_DotCue::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	FVector Location = Target->GetActorLocation();
	
	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(Target, HitParticle, Location);
		UGameplayStatics::SpawnSoundAtLocation(Target, HitSound, Location);
	}
	return false;
}
