// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "CB_GuardAbility.generated.h"

class ACB_BaseCharacter;
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
	
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer TagContainer;

	UPROPERTY()
	TObjectPtr<ACB_BaseCharacter> BaseCharacter;
};
