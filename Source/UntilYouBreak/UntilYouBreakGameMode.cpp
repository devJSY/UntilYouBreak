// Copyright Epic Games, Inc. All Rights Reserved.

#include "UntilYouBreakGameMode.h"
#include "UntilYouBreakCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUntilYouBreakGameMode::AUntilYouBreakGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
