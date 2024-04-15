// Fill out your copyright notice in the Description page of Project Settings.

#include "Attribute/CB_CharacterAttributeSet.h"
#include "GameplayEffectExtension.h"

UCB_CharacterAttributeSet::UCB_CharacterAttributeSet()
{
	InitHealth(GetMaxHealth());
	InitStamina(GetMaxStamina());
}

void UCB_CharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());

	if (Attribute == GetStaminaAttribute())
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
}

void UCB_CharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
}
