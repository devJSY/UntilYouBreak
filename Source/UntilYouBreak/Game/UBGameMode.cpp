// Copyright Epic Games, Inc. All Rights Reserved.

#include "UBGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/UBPlayerController.h"
#include "GameData/UBGameSingleton.h"
#include "Game/UBStageSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UBGameInstance.h"

AUBGameMode::AUBGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/UntilYouBreak/Blueprint/BP_UBCharacterPlayer.BP_UBCharacterPlayer_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//// UBPlayerController 헤더 include 없이 PlayerControllerClass 설정
	// static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/UntilYouBreak.UBPlayerController'"));
	// if (PlayerControllerClassRef.Class)
	//{
	//	PlayerControllerClass = PlayerControllerClassRef.Class;
	// }

	CurrentStageLevel = 1;
	RemainingEnemyCount = 1;
	bIsCleared = false;
}

void AUBGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SaveGameInstance = Cast<UUBStageSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Stage"), 0));

	if (SaveGameInstance)
	{
		CurrentStageLevel = ++(SaveGameInstance->StageLevel);
	}
	else
	{
		SaveGameInstance = NewObject<UUBStageSaveGame>();
		SaveGameInstance->StageLevel = 1;
	}

	RemainingEnemyCount = UUBGameSingleton::Get().GetStageLevel(CurrentStageLevel).EnemyCount;
}

void AUBGameMode::OnEnemyDestroyed()
{
	--RemainingEnemyCount;

	AUBPlayerController* UBPlayerController = Cast<AUBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (UBPlayerController)
	{
		UBPlayerController->GameScoreChanged(RemainingEnemyCount);
	}

	if (0 >= RemainingEnemyCount)
	{
		bIsCleared = true;

		if (UBPlayerController)
		{
			if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Stage"), 0))
			{
				UE_LOG(LogUBPlayerController, Error, TEXT("Save Game Error!"));
			}

			UBPlayerController->GameClear();
		}
	}
}

void AUBGameMode::OnPlayerDead()
{
	UUBGameInstance* UBGameInstance = Cast<UUBGameInstance>(GetWorld()->GetGameInstance());
	if (UBGameInstance)
	{
		UBGameInstance->DeleteStageSaveGame();
	}

	AUBPlayerController* UBPlayerController = Cast<AUBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (UBPlayerController)
	{

		UBPlayerController->GameOver();
	}
}

bool AUBGameMode::IsGameCleared()
{
	return bIsCleared;
}
