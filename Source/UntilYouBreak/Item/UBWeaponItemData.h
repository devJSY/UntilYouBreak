// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/UBItemData.h"
#include "GameData/UBCharacterStat.h"
#include "UBWeaponItemData.generated.h"

/**
 *
 */
UCLASS()
class UNTILYOUBREAK_API UUBWeaponItemData : public UUBItemData
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("UBWeaponItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Stat)
	FUBCharacterStat ModifierStat;
};
