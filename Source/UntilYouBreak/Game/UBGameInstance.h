// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameData/UBCharacterStat.h"
#include "UBGameInstance.generated.h"

/**
 *
 */
UCLASS()
class UNTILYOUBREAK_API UUBGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UUBGameInstance();

public:
	virtual void Init() override;

	// Stage Save Game
public:
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable, Category = SaveGame, Meta = (DisplayName = "OnDeleteStageSaveGameCpp"))
	void DeleteStageSaveGame();

	// Character Info
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FUBCharacterStat SavedBaseStat;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FUBCharacterStat SavedModifierStat;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float SavedCurrentHp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 SavedCurrentExp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 SavedCurrentLevel;

	class UUBWeaponItemData* WeaponItemData;

	UFUNCTION(BlueprintCallable, Category = CharacterStat, Meta = (DisplayName = "OnResetCharacterStatCpp"))
	void ResetCharacterStat();

	void SaveCharacterInfo(class UUBCharacterStatComponent* InStatComponent, class UUBWeaponItemData* InWeaponItemData);
	void LoadCharacterInfo(class UUBCharacterStatComponent* InStatComponent);

	class UUBWeaponItemData* GetWeaponItemData() const
	{
		return WeaponItemData;
	};
};
