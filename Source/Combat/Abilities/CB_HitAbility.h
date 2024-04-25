// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CB_HitAbility.generated.h"

class ACB_BaseCharacter;
class UAnimMontage;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class COMBAT_API UCB_HitAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCB_HitAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	void PlayGameplayCue(const FGameplayEventData* TriggerEventData);

	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnBlendOutCallback();

	UFUNCTION()
	void OnCancelledCallback();

	UFUNCTION()
	void OnInterruptedCallback();

private:
	UPROPERTY()
	TObjectPtr<ACB_BaseCharacter> BaseCharacter;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> HitEffect;

};
