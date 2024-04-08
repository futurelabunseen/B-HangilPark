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
}

void UCB_CharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}

}
