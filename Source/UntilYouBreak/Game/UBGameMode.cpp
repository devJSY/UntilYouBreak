// Copyright Epic Games, Inc. All Rights Reserved.

#include "UBGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/UBPlayerController.h"

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

	ClearScore = 3;
	CurrentScore = 0;
	bIsCleared = false;
}

void AUBGameMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	CurrentScore = NewPlayerScore;

	AUBPlayerController* UBPlayerController = Cast<AUBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (UBPlayerController)
	{
		UBPlayerController->GameScoreChanged(CurrentScore);
	}

	if (CurrentScore >= ClearScore)
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
