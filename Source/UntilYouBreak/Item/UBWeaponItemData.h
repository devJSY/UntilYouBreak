// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/UBItemData.h"
#include "UBWeaponItemData.generated.h"

/**
 *
 */
UCLASS()
class UNTILYOUBREAK_API UUBWeaponItemData : public UUBItemData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;
};
