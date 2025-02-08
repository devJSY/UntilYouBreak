// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/UBGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Character/UBCharacterStatComponent.h"
#include "GameData/UBGameSingleton.h"

UUBGameInstance::UUBGameInstance()
{
}

void UUBGameInstance::Init()
{
	Super::Init();

	ResetCharacterStat();
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

void UUBGameInstance::ResetCharacterStat()
{
	SavedBaseStat = UUBGameSingleton::Get().GetCharacterStat(1);
	SavedModifierStat = FUBCharacterStat();
	SavedCurrentHp = SavedBaseStat.MaxHp;
	SavedCurrentExp = 0;
	SavedCurrentLevel = 1;
}

void UUBGameInstance::SaveCharacterStat(UUBCharacterStatComponent* StatComponent)
{
	if (!StatComponent)
		return;

	SavedCurrentLevel = StatComponent->GetCurrentLevel();
	SavedCurrentHp = StatComponent->GetCurrentHp();
	SavedCurrentExp = StatComponent->GetCurrentExp();
	SavedBaseStat = StatComponent->GetBaseStat();
	SavedModifierStat = StatComponent->GetModifierStat();
}

void UUBGameInstance::LoadCharacterStat(UUBCharacterStatComponent* StatComponent)
{
	if (!StatComponent)
		return;

	StatComponent->SetLevelStat(SavedCurrentLevel);
	StatComponent->SetHp(SavedCurrentHp);
	StatComponent->GainExp(SavedCurrentExp);
	StatComponent->SetBaseStat(SavedBaseStat);
	StatComponent->SetModifierStat(SavedModifierStat);
}