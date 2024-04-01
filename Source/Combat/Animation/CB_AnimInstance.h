// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CB_AnimInstance.generated.h"

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
	TObjectPtr<class ACharacter> Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
		Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterMovementComponent> CharacterMovement;
	
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
};
