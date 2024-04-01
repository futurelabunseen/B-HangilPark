// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CB_DodgeAbility.generated.h"

class UAnimMontage;
class ACB_BaseCharacter;
/**
 * 
 */
UCLASS()
class COMBAT_API UCB_DodgeAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCB_DodgeAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UFUNCTION()
	void OnCompleteCallback();

private:
	void ImmediateRotateActor();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY()
	TObjectPtr<ACB_BaseCharacter> BaseCharacter;
};
