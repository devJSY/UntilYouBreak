// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/UBGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Character/UBCharacterStatComponent.h"
#include "GameData/UBGameSingleton.h"
#include "Engine/AssetManager.h"
#include "Item/UBWeaponItemData.h"

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

	// DefaultAsset
	FPrimaryAssetId AssetId = FPrimaryAssetId(FPrimaryAssetType("UBWeaponItemData"), FName("UBW_DefaultWeapon"));
	FSoftObjectPtr	AssetPtr(UAssetManager::Get().GetPrimaryAssetPath(AssetId));

	if (!AssetPtr.IsNull())
	{
		if (AssetPtr.IsPending())
		{
			AssetPtr.LoadSynchronous();
		}
		WeaponItemData = Cast<UUBWeaponItemData>(AssetPtr.Get());
		ensure(WeaponItemData != nullptr);
	}
}

void UUBGameInstance::SaveCharacterInfo(UUBCharacterStatComponent* InStatComponent, UUBWeaponItemData* InWeaponItemData)
{
	if (InStatComponent)
	{
		SavedCurrentLevel = InStatComponent->GetCurrentLevel();
		SavedCurrentHp = InStatComponent->GetCurrentHp();
		SavedCurrentExp = InStatComponent->GetCurrentExp();
		SavedBaseStat = InStatComponent->GetBaseStat();
		SavedModifierStat = InStatComponent->GetModifierStat();
	}

	if (InWeaponItemData)
	{
		WeaponItemData = InWeaponItemData;
	}
}

void UUBGameInstance::LoadCharacterInfo(UUBCharacterStatComponent* InStatComponent)
{
	if (InStatComponent)
	{
		InStatComponent->SetLevelStat(SavedCurrentLevel);
		InStatComponent->SetHp(SavedCurrentHp);
		InStatComponent->GainExp(SavedCurrentExp);
		InStatComponent->SetBaseStat(SavedBaseStat);
		InStatComponent->SetModifierStat(SavedModifierStat);
	}
}
