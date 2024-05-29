// Fill out your copyright notice in the Description page of Project Settings.


#include "Cue/CB_HitCue.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UCB_HitCue::UCB_HitCue()
{
	HitParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("HitParticle"));
}

bool UCB_HitCue::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();

	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(Target, HitParticle, HitResult->ImpactPoint);
		UGameplayStatics::SpawnSoundAtLocation(Target, HitSound, HitResult->ImpactPoint);
	}
	return false;
}
