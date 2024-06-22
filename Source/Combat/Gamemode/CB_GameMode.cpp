// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/CB_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/CB_GameInstance.h"
#include "Controller/CB_PlayerController.h"
#include "Blueprint/UserWidget.h"

void ACB_GameMode::AsyncLevelLoad(const FString& LevelDir, const FString& LevelName)
{
	LoadingOverlayActivate();

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
	GameWinCheck();
}

void ACB_GameMode::LoadingOverlayActivate()
{
	if (IsValid(LoadingOverlayClass))
	{
		LoadingOverlay = CreateWidget(GetWorld(), LoadingOverlayClass);
		if (IsValid(LoadingOverlay))
			LoadingOverlay->AddToViewport();
	}
}

void ACB_GameMode::GameWinCheck()
{
	UCB_GameInstance* Instance = Cast<UCB_GameInstance>(GetGameInstance());
	if (Instance->GameWinCheck())
	{
		ACB_PlayerController* Controller = Cast<ACB_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		Controller->SetPlayerInputMode(true);

		if (IsValid(WinOverlayClass))
		{
			WinOverlay = CreateWidget(GetWorld(), WinOverlayClass);
			if (IsValid(WinOverlay))
				WinOverlay->AddToViewport();
		}
	}
}

void ACB_GameMode::AsyncLevelLoadFinished(const FString& LevelName)
{
	const FString Options;
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), true, Options);
}
