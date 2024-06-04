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
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnCancelCallback();

	void DoCameraShake();

private:
	FORCEINLINE FName CheckSectionName(const float Theta);

	void ApplyGameplayEffect(const FGameplayEffectSpecHandle& EffectSpecHandle);
	void PlayGameplayCue(const FGameplayEffectSpecHandle& EffectSpecHandle, const FHitResult& HitResult);

	void PlayMontgage(const FName& SectionName);
	bool ParryingCheck(const FName& SectionName);

private:
	UPROPERTY()
	TObjectPtr<ACB_BaseCharacter> BaseCharacter;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> HitEffect;

	bool bIsBlocking = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> ParryEffect;
};
