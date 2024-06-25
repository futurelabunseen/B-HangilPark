// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Data/StateTag.h"
#include "CB_EnemyDodgeAbility.generated.h"


/**
 * 
 */
UCLASS()
class COMBAT_API UCB_EnemyDodgeAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCB_EnemyDodgeAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

private:
	FName RandomSection();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<FName> DodgeMontageSections;
};
