// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CB_TraceTargetActor.generated.h"

UCLASS()
class COMBAT_API ACB_TraceTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;
};
