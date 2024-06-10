// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CB_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API ACB_GameMode : public AGameMode
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void AsyncLevelLoad(const FString& LevelDir, const FString& LevelName);

protected:
	virtual void BeginPlay() override;

private:
	void AsyncLevelLoadFinished(const FString& LevelName);

};
