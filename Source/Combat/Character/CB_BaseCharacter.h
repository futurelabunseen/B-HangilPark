// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "CB_BaseCharacter.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UCB_LockOnComponent;
class ACB_BaseWeapon;

UCLASS()
class COMBAT_API ACB_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACB_BaseCharacter();

	void LockOn();
	bool IsLocked();
	UFUNCTION()
	void LockChange(float Axis);
	
	void AddGameplayTag(FGameplayTag Tag);
	void RemoveGameplayTag(FGameplayTag Tag);
public:
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override 
	{ return ASC; }
	FORCEINLINE ACB_BaseWeapon* GetWeapon() const { return Weapon; }
	FORCEINLINE bool GetIsEquipped() const { return bIsEquipped; }
	FORCEINLINE void FlipIsEquipped() { bIsEquipped = !bIsEquipped; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> ASC = nullptr;

	UPROPERTY(EditAnywhere, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	UPROPERTY(VisibleAnywhere, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCB_LockOnComponent> LockOnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACB_BaseWeapon> Weapon;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACB_BaseWeapon> WeaponClass;

	// 나중에 지울 것
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsEquipped = false;
};
