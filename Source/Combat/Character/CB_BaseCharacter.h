// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "Interface/CB_CombatInterface.h"
#include "CB_BaseCharacter.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UCB_LockOnComponent;
class ACB_BaseWeapon;
class UCB_CharacterAttributeSet;
class UCB_DA_Weapon;

namespace WalkSpeed
{
	const float Walk = 300.f;
	const float Run = 450.f;
};

UCLASS()
class COMBAT_API ACB_BaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICB_CombatInterface
{
	GENERATED_BODY()

public:
	ACB_BaseCharacter();

	void LockOn();
	bool IsLocked();
	UFUNCTION()
	void LockChange(float Axis);
	
	UFUNCTION()
	bool HasGameplayTag(FGameplayTag Tag) const;

	virtual FVector GetSocketLocation(const FName SocketName) override;
	virtual FVector GetWeaponSocketLocation(const FName SocketName) override;

	virtual void Dead();

	UFUNCTION(BlueprintCallable)
	bool GetIsGuard() const { return bIsGuard; }
	
	UFUNCTION(BlueprintCallable)
	void SetIsGuard(const bool IsGaurd);

public:
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	FORCEINLINE ACB_BaseWeapon* GetWeapon() const { return Weapon; }
	FORCEINLINE UCB_DA_Weapon* GetWeaponData() const { return WeaponData; }
	FORCEINLINE void AddUniqueGameplayTag(FGameplayTag Tag);
	FORCEINLINE void RemoveUniqueGameplayTag(FGameplayTag Tag);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> ASC = nullptr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCB_LockOnComponent> LockOnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACB_BaseWeapon> Weapon;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCB_DA_Weapon> WeaponData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCB_CharacterAttributeSet> AttributeSet = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsGuard = false;
};
