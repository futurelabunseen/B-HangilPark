// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Data/StateTag.h"
#include "Interface/CB_CameraShakeInterface.h"
#include "CB_PlayerController.generated.h"

struct FInputActionValue;
class UCB_PDA_Input;
class UCB_LegacyCameraShake;

DECLARE_DELEGATE_OneParam(FLockChangeDelegate, float);

UCLASS()
class COMBAT_API ACB_PlayerController : public APlayerController, public ICB_CameraShakeInterface
{
	GENERATED_BODY()
public:
	virtual void DoCameraShake() override;

	UFUNCTION(BlueprintCallable)
	void SetPlayerInputMode(bool bInputMode);

	UFUNCTION(BlueprintCallable)
	void ShowOverlay(bool bIsShow);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void LockOn();
	void Guard(bool bIsActiave);

	void InputPressed(const FGameplayTag Tag);
	void InputReleased(const FGameplayTag Tag);

public:
	FLockChangeDelegate LockChangeDelegate;

private:
	UPROPERTY(EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCB_PDA_Input> InputData = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCB_LegacyCameraShake> CameraShake;
};
