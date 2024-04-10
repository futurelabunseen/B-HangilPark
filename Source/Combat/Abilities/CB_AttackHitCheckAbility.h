// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CB_AttackHitCheckAbility.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class COMBAT_API UCB_AttackHitCheckAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCB_AttackHitCheckAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float Damage = 20.f;
};
