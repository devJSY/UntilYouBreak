// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/UBPlayerController.h"
#include "UI/UBHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Game/UBSaveGame.h"

DEFINE_LOG_CATEGORY(LogUBPlayerController);

AUBPlayerController::AUBPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUBHUDWidget> UBHUDWidgetRef(TEXT("/Game/UntilYouBreak/UI/WBP_UBHUD.WBP_UBHUD_C"));
	if (UBHUDWidgetRef.Class)
	{
		UBHUDWidgetClass = UBHUDWidgetRef.Class;
	}
}

void AUBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SaveGameInstance = Cast<UUBSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));

	if (SaveGameInstance)
	{
		SaveGameInstance->RetryCount++;
	}
	else
	{
		SaveGameInstance = NewObject<UUBSaveGame>();
		SaveGameInstance->RetryCount = 0;
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}

void AUBPlayerController::GameScoreChanged(int32 NewScore)
{
	K2_OnScoreChanged(NewScore);
}

void AUBPlayerController::StageClear()
{
	K2_OnStageClear();
}

void AUBPlayerController::GameClear()
{
	K2_OnGameClear();
}

void AUBPlayerController::GameOver()
{
	K2_OnGameOver();

	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		UE_LOG(LogUBPlayerController, Error, TEXT("Save Game Error!"));
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}
