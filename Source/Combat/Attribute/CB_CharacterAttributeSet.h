// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Data/StateTag.h"
#include "CB_CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealthDelegate);

namespace SPEED
{
	const float WALK = 200.f;
	const float RUN = 450.f;
};

UCLASS()
class COMBAT_API UCB_CharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	friend class UCB_DamageCalculation;
	friend class UCB_AttackDamage;
public:
	UCB_CharacterAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	ATTRIBUTE_ACCESSORS(UCB_CharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UCB_CharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UCB_CharacterAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UCB_CharacterAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UCB_CharacterAttributeSet, StaminaRegenRate);

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina = 100.f;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StaminaRegenRate = .5f;

public:
	mutable FOutOfHealthDelegate OnOutOfHealth;
private:
	bool bOutOfHealth = false;
};
