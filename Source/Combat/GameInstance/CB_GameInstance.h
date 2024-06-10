// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CB_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UCB_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	int32 GetWinCnt() const { return WinCnt; }
	
	UFUNCTION()
	void IncWinCnt() { ++WinCnt; }

private:
	UPROPERTY()
	int32 WinCnt = 0;
};
