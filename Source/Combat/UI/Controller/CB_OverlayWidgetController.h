// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Controller/CB_WidgetController.h"
#include "CB_OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealtChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealtChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChangedSignature, float, NewStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaChangedSignature, float, NewMaxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaRegenRateChangedSignature, float, NewStaminaRegenRate);

UCLASS(BlueprintType, Blueprintable)
class COMBAT_API UCB_OverlayWidgetController : public UCB_WidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void StaminaChanged(const FOnAttributeChangeData& Data) const;
	void MaxStaminaChanged(const FOnAttributeChangeData& Data) const;
	void StaminaRegenRateChanged(const FOnAttributeChangeData& Data) const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealtChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMaxHealtChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMaxStaminaChangedSignature OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaRegenRateChangedSignature OnStaminaRegenRateChanged;
};
