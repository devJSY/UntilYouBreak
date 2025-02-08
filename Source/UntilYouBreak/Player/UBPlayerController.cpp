// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/UBPlayerController.h"
#include "UI/UBHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Game/UBSaveGame.h"
#include "Game/UBGameInstance.h"
#include "Character/UBCharacterBase.h"

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

	// 스탯 불러오기
	AUBCharacterBase* ControlledPlayer = Cast<AUBCharacterBase>(GetPawn());
	UUBGameInstance*  GameInst = Cast<UUBGameInstance>(GetGameInstance());
	if (ControlledPlayer && GameInst)
	{
		GameInst->LoadCharacterStat(ControlledPlayer->GetStatComponent());
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

	// 스탯 저장하기
	AUBCharacterBase* ControlledPlayer = Cast<AUBCharacterBase>(GetPawn());
	UUBGameInstance*  GameInst = Cast<UUBGameInstance>(GetGameInstance());
	if (ControlledPlayer && GameInst)
	{
		GameInst->SaveCharacterStat(ControlledPlayer->GetStatComponent());
	}
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

	// 스탯 리셋
	UUBGameInstance* GameInst = Cast<UUBGameInstance>(GetGameInstance());
	if (GameInst)
	{
		GameInst->ResetCharacterStat();
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}
