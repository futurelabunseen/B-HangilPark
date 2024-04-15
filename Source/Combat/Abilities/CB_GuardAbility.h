// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CB_GuardAbility.generated.h"

class UAnimMontage;
/**
 * 
 */
UCLASS()
class COMBAT_API UCB_GuardAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCB_GuardAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> GuardMontage;
};
