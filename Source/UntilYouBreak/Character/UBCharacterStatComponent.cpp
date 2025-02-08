// Fill out your copyright notice in the Description page of Project Settings.

#include "UBCharacterStatComponent.h"
#include "GameData/UBGameSingleton.h"

UUBCharacterStatComponent::UUBCharacterStatComponent()
{
	CurrentLevel = 1;
	CurrentExp = 0.0f;

	bWantsInitializeComponent = true;
}

void UUBCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UUBCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UUBGameSingleton::Get().CharacterMaxLevel);
	SetBaseStat(UUBGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f);
	OnLevelChanged.Broadcast(CurrentLevel);
}

float UUBCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= 0.0f)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UUBCharacterStatComponent::GainExp(int32 InAmountExp)
{
	const int32 ActualExp = FMath::Clamp<int32>(InAmountExp, 0, InAmountExp);
	CurrentExp += ActualExp;

	// ·¹º§¾÷
	while (CurrentExp >= UUBGameSingleton::Get().GetCharacterExp(CurrentLevel).MaxExp)
	{
		CurrentExp -= UUBGameSingleton::Get().GetCharacterExp(CurrentLevel).MaxExp;
		SetLevelStat(CurrentLevel + 1);
	}

	OnExpChanged.Broadcast(CurrentExp, UUBGameSingleton::Get().GetCharacterExp(CurrentLevel).MaxExp);
}

void UUBCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}
