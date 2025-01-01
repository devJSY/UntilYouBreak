// Copyright Epic Games, Inc. All Rights Reserved.

#include "UntilYouBreakGameMode.h"
#include "Character/UntilYouBreakCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUntilYouBreakGameMode::AUntilYouBreakGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// UBPlayerController 헤더 include 없이 PlayerControllerClass 설정
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/UntilYouBreak.UBPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
