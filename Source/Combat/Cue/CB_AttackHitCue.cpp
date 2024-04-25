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
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();

//	UE_LOG(LogTemp, Warning, TEXT("HitResult : %f, %f, %f"), HitResult->ImpactPoint.X, HitResult->ImpactPoint.Y, HitResult->ImpactPoint.Z);
//	DrawDebugSphere(GetWorld(), HitResult->ImpactPoint, 10.f, 5, FColor::Orange, false, 1.f);

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
	}
	return false;
}
