// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "CB_PlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class COMBAT_API ACB_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACB_PlayerState();

public:
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{ return ASC; }
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
