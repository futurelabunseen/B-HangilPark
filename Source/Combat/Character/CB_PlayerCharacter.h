// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CB_BaseCharacter.h"
#include "Interface/CB_TrailInterface.h"
#include "CB_PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGameplayAbility;
class UCB_TrailComponent;

UCLASS()
class COMBAT_API ACB_PlayerCharacter : public ACB_BaseCharacter, public ICB_TrailInterface
{
	GENERATED_BODY()

public:
	ACB_PlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void TrailStart(FGameplayTag Tag) override;

	virtual void SetWarpTarget() override;

	virtual void Dead() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent>	SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCB_TrailComponent> TrailComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> DiedOverlayClass;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> DiedOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CommandOverlayClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> CommandOverlay;
};
