// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Tags/StateTag.h"
#include "CB_PlayerController.generated.h"

struct FInputActionValue;
class UCB_PDA_Input;

DECLARE_DELEGATE_OneParam(FLockChangeDelegate, float);

UCLASS()
class COMBAT_API ACB_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void LockOn();

	void InputPressed(const FGameplayTag Tag);
	void InputReleased(const FGameplayTag Tag);

public:
	FLockChangeDelegate LockChangeDelegate;

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UCB_PDA_Input> InputData = nullptr;

};
