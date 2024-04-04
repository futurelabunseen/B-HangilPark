// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Tags/StateTag.h"
#include "CB_AnimInstance.generated.h"

class ACB_BaseCharacter;
class UCharacterMovementComponent;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class COMBAT_API UCB_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
		Category = "Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACB_BaseCharacter> Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
		Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, 
		Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
		Category = "Movement", meta = (AllowPrivateAccess = "true"))
	uint8 bIsFalling : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
		Category = "Movement", meta = (AllowPrivateAccess = "true"))
	uint8 bShouldMove : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
		Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveForward;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
		Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
		Category = "Flag", meta = (AllowPrivateAccess = "true"))
	bool IsEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> ASC = nullptr;
};
