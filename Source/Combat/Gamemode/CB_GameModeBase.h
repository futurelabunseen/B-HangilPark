// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CB_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API ACB_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AsyncLevelLoad(const FString& LevelDir, const FString& LevelName, const FString& Options);

private:
	void AsyncLevelLoadFinished(const FString& LevelName, const FString& Options);
};
