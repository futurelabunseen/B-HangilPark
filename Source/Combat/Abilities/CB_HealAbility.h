// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CB_HealAbility.generated.h"

class ACB_BaseCharacter;
class UAnimMontage;

/**
 * 
 */
UCLASS()
class COMBAT_API UCB_HealAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCB_HealAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnCancelCallback();

private:
	UPROPERTY()
	TObjectPtr<ACB_BaseCharacter> BaseCharacter;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> HealMontage;
};
