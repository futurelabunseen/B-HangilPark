// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_GameInstance.h"

UCB_GameInstance::UCB_GameInstance()
{
	CheckLevelClear.Add(FString("Shore"), false);
	CheckLevelClear.Add(FString("Temple"), false);
}

void UCB_GameInstance::ClearThisLevel(const FString& LevelName)
{
	// Map �����̳� Add �� Key ���� �����ϸ� ��ü
	if (CheckLevelClear.Find(LevelName))
		CheckLevelClear.Add(LevelName, true);
}

bool UCB_GameInstance::IsMapClear(const FString& LevelName)
{
	// FindRef()�� null �˻�� ���� ���� Ȯ�� ���� ���� ȣ���ص� �����մϴ�.[��ó : https://jhtop0419.tistory.com/46]
	return CheckLevelClear.FindRef(LevelName);
}
