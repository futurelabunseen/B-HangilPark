// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Controller/CB_OverlayWidgetController.h"
#include "Attribute/CB_CharacterAttributeSet.h"

void UCB_OverlayWidgetController::BroadcastInitialValues()
{
	const UCB_CharacterAttributeSet* AS = CastChecked<UCB_CharacterAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AS->GetHealth());
	OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	OnStaminaChanged.Broadcast(AS->GetStamina());
	OnMaxStaminaChanged.Broadcast(AS->GetMaxStamina());
	OnStaminaRegenRateChanged.Broadcast(AS->GetStaminaRegenRate());
}

void UCB_OverlayWidgetController::BindCallbacksToDependencies()
{
	const UCB_CharacterAttributeSet* AS = CastChecked<UCB_CharacterAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AS->GetHealthAttribute()).AddUObject(this, &UCB_OverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AS->GetMaxHealthAttribute()).AddUObject(this, &UCB_OverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AS->GetStaminaAttribute()).AddUObject(this, &UCB_OverlayWidgetController::StaminaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AS->GetMaxStaminaAttribute()).AddUObject(this, &UCB_OverlayWidgetController::MaxStaminaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AS->GetStaminaRegenRateAttribute()).AddUObject(this, &UCB_OverlayWidgetController::StaminaRegenRateChanged);
}

void UCB_OverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UCB_OverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UCB_OverlayWidgetController::StaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnStaminaChanged.Broadcast(Data.NewValue);
}

void UCB_OverlayWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxStaminaChanged.Broadcast(Data.NewValue);
}

void UCB_OverlayWidgetController::StaminaRegenRateChanged(const FOnAttributeChangeData& Data) const
{
	OnStaminaRegenRateChanged.Broadcast(Data.NewValue);
}
