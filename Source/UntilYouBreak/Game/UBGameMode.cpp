// Copyright Epic Games, Inc. All Rights Reserved.

#include "UBGameMode.h"
#include "UObject/ConstructorHelpers.h"

AUBGameMode::AUBGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/UntilYouBreak/Blueprint/BP_UBCharacterPlayer.BP_UBCharacterPlayer_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// UBPlayerController ��� include ���� PlayerControllerClass ����
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/UntilYouBreak.UBPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
