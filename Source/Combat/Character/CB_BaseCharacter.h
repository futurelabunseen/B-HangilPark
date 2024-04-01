// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CB_BaseCharacter.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
UCLASS()
class COMBAT_API ACB_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACB_BaseCharacter();

public:
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override 
	{ return ASC; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> ASC = nullptr;

	UPROPERTY(EditAnywhere, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> Abilities;



};
