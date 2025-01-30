// Copyright Epic Games, Inc. All Rights Reserved.

#include "UBGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/UBPlayerController.h"
#include "GameData/UBGameSingleton.h"

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

	CurrentStageNum = 3;
	RemainingEnemyCount = 1;
	bIsCleared = false;
}

void AUBGameMode::BeginPlay()
{
	Super::BeginPlay();

	RemainingEnemyCount = UUBGameSingleton::Get().GetStageLevel(CurrentStageNum).EnemyCount;
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
			UBPlayerController->GameClear();
		}
	}
}

void AUBGameMode::OnPlayerDead()
{
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
