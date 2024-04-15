// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CB_HUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UCB_OverlayWidgetController;
class UCB_UserWidget;
struct FWidgetControllerParams;

UCLASS()
class COMBAT_API ACB_HUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UCB_UserWidget>  OverlayWidget;

	UCB_OverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCB_UserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UCB_OverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCB_OverlayWidgetController> OverlayWidgetControllerClass;
};
