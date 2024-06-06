// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ACB_GameModeBase::AsyncLevelLoad(const FString& LevelDir, const FString& LevelName, const FString& Options)
{
	LoadPackageAsync(LevelDir + LevelName,
		FLoadPackageAsyncDelegate::CreateLambda([=, this](
			const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result) {
				if (Result == EAsyncLoadingResult::Succeeded)
					AsyncLevelLoadFinished(LevelName, Options);
			}), 0, PKG_ContainsMap);
}

void ACB_GameModeBase::AsyncLevelLoadFinished(const FString& LevelName, const FString& Options)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), true, Options);
}
