// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/CB_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/CB_GameInstance.h"

void ACB_GameMode::AsyncLevelLoad(const FString& LevelDir, const FString& LevelName)
{
	LoadPackageAsync(LevelDir + LevelName,
		FLoadPackageAsyncDelegate::CreateLambda([=, this](
			const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result) {
				if (Result == EAsyncLoadingResult::Succeeded)
					AsyncLevelLoadFinished(LevelName);
			}), 0, PKG_ContainsMap);
}

void ACB_GameMode::BeginPlay()
{
	Super::BeginPlay();
	static const uint8 BossCnt = 2; 
	UCB_GameInstance* Instance = Cast<UCB_GameInstance>(GetGameInstance());
	UE_LOG(LogTemp, Warning, TEXT("%d"), Instance->GetWinCnt());

	
	if (Instance->GetWinCnt() == BossCnt)
	{
		// ½Â¸® À§Á¬ È£Ãâ
		UE_LOG(LogTemp, Error, TEXT("Win!"));
	}
}

void ACB_GameMode::AsyncLevelLoadFinished(const FString& LevelName)
{
	const FString Options;
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), true, Options);
}
