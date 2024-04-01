// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CB_ComboAbility.generated.h"

class UAnimMontage;
class ACB_BaseCharacter;
/**
 * 
 */
UCLASS()
class COMBAT_API UCB_ComboAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCB_ComboAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
    void PlayMontage(FGameplayEventData Data);

private:
	void ImmediateRotateActor();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	uint8 IsFirstAttack : 1;

	UPROPERTY()
	TObjectPtr<ACB_BaseCharacter> BaseCharacter;
};
