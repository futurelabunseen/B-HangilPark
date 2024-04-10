// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CB_BaseCharacter.h"
#include "CB_PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGameplayAbility;
/**
 * 
 */
UCLASS()
class COMBAT_API ACB_PlayerCharacter : public ACB_BaseCharacter
{
	GENERATED_BODY()

public:
	ACB_PlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION()
	void InputPressed(const FGameplayTagContainer& GameplayTagContainer);

	UFUNCTION()
	void InputReleased(const FGameplayTagContainer& GameplayTagContainer);

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent>	SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

};
