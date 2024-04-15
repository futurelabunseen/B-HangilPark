// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "CB_AttackHitCue.generated.h"

class UParticleSystem;

UCLASS()
class COMBAT_API UCB_AttackHitCue : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	UCB_AttackHitCue();

	virtual bool OnExecute_Implementation(AActor* Target,
		const FGameplayCueParameters& Parameters) const override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> ParticleSystem;
};
