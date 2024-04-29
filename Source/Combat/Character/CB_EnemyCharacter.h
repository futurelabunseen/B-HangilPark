// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CB_BaseCharacter.h"
#include "UI/Controller/CB_OverlayWidgetController.h"
#include "CB_EnemyCharacter.generated.h"

class UGameplayEffect;
class UWidgetComponent;

UCLASS()
class COMBAT_API ACB_EnemyCharacter : public ACB_BaseCharacter
{
	GENERATED_BODY()
public:
	ACB_EnemyCharacter();
	void SetOutLine(bool bIsShow);
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(BlueprintAssignable)
	FOnHealtChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMaxHealtChangedSignature OnMaxHealthChanged;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> InitStatEffect;

	UPROPERTY(EditAnywhere)
	float Level = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
