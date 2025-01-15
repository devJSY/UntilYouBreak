// Fill out your copyright notice in the Description page of Project Settings.

#include "GameData/UBGameSingleton.h"
#include "Engine\\DataTable.h"

DEFINE_LOG_CATEGORY(LogUBGameSingleton)

UUBGameSingleton::UUBGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/UntilYouBreak/GameData/UBCharacter_StatTable.UBCharacter_StatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable, [](uint8* Value) {
			return *reinterpret_cast<FUBCharacterStat*>(Value);
		});
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UUBGameSingleton& UUBGameSingleton::Get()
{
	UUBGameSingleton* Singleton = CastChecked<UUBGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogUBGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UUBGameSingleton>();
}
