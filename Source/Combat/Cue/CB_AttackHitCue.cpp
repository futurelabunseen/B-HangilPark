// Fill out your copyright notice in the Description page of Project Settings.

#include "Cue/CB_AttackHitCue.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UCB_AttackHitCue::UCB_AttackHitCue()
{
	ParticleSystem = CreateDefaultSubobject<UParticleSystem>(TEXT("ParticleSystem"));
}

bool UCB_AttackHitCue::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Target->GetName());

	UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem,
		Target->GetActorLocation(), FRotator::ZeroRotator, true);

	/*const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem,
			HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}
	else
	{
		for (const auto& TargetActor : Parameters.EffectContext.Get()->GetActors())
		{
			if (TargetActor.Get())
				UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem,
					TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, true);
		}
	}*/
	return false;
}
