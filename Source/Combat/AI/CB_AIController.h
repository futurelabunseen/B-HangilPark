// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CB_AIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class COMBAT_API ACB_AIController : public AAIController
{
	GENERATED_BODY()

public:
	ACB_AIController();

	UFUNCTION(BlueprintCallable)
	void StopAI();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
