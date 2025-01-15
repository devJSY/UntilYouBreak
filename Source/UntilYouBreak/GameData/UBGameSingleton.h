// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UBCharacterStat.h"
#include "UBGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUBGameSingleton, Error, All);

/**
 *
 */
UCLASS()
class UNTILYOUBREAK_API UUBGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UUBGameSingleton();
	static UUBGameSingleton& Get();

	// Character Stat Data Section
public:
	FORCEINLINE FUBCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FUBCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FUBCharacterStat> CharacterStatTable;
};
