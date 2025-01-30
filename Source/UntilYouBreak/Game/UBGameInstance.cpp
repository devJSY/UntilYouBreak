// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/UBGameInstance.h"
#include "Kismet/GameplayStatics.h"

UUBGameInstance::UUBGameInstance()
{
}

void UUBGameInstance::Shutdown()
{
	Super::Shutdown();

	DeleteStageSaveGame();
}

void UUBGameInstance::DeleteStageSaveGame()
{
	const FString SlotName = TEXT("Stage");
	const int32	  UserIndex = 0;

	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		if (!UGameplayStatics::DeleteGameInSlot(SlotName, UserIndex))
		{
			UE_LOG(LogTemp, Error, TEXT("Delete Game Error!"));
		}
	}
}
