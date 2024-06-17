// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_GameInstance.h"

UCB_GameInstance::UCB_GameInstance()
{
	CheckLevelClear.Add(FString("Shore"), false);
	CheckLevelClear.Add(FString("Temple"), false);
}

void UCB_GameInstance::ClearThisLevel(const FString& LevelName)
{
	// Map 컨테이너 Add 시 Key 값이 존재하면 대체
	if (CheckLevelClear.Find(LevelName))
		CheckLevelClear.Add(LevelName, true);
}

bool UCB_GameInstance::IsMapClear(const FString& LevelName)
{
	// FindRef()는 null 검사와 같은 안전 확인 절차 없이 호출해도 안전합니다. [출처 : https://jhtop0419.tistory.com/46]
	return CheckLevelClear.FindRef(LevelName);
}
