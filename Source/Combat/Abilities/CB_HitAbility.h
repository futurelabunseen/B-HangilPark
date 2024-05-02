// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CB_HitAbility.generated.h"

USTRUCT()
struct FHitData
{
	GENERATED_BODY()
	TPair<float, float> Range;
	FName Name;
};

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
	void OnCancelCallback();

private:
	float CheckTheta(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
	FORCEINLINE FName CheckSectionName(const float Theta) { return (0 <= Theta && Theta <= 90.f) ? FName("Fwd") : FName("Bwd"); }

private:
	UPROPERTY()
	TObjectPtr<ACB_BaseCharacter> BaseCharacter;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> HitEffect;

};
