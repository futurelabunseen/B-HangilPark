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
	UCB_GameInstance();

	UFUNCTION(BlueprintCallable)
	int32 GetWinCnt() const { return WinCnt; }
	
	UFUNCTION(BlueprintCallable)
	void IncWinCnt() { ++WinCnt; }

	void ClearThisLevel(const FString& LevelName);

	UFUNCTION(BlueprintCallable)
	bool IsMapClear(const FString& LevelName);

	FORCEINLINE bool GameWinCheck() const { return (MaxWin == WinCnt); }

private:
	UPROPERTY()
	int32 WinCnt = 0;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TMap<FString, bool> CheckLevelClear;
	
	const uint8 MaxWin = 2;
};
