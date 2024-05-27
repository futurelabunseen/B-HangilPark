// Fill out your copyright notice in the Description page of Project Settings.


#include "Cue/CB_HitCue.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UCB_HitCue::UCB_HitCue()
{
	ParticleSystem = CreateDefaultSubobject<UParticleSystem>(TEXT("ParticleSystem"));
}

bool UCB_HitCue::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();

	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem,
			HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}
	return false;
}
