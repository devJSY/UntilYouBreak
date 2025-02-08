// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UBHUDWidget.h"
#include "Interface/UBCharacterHUDInterface.h"
#include "UBHpBarWidget.h"
#include "UBExpBarWidget.h"
#include "UBCharacterStatWidget.h"

void UUBHUDWidget::UpdateStat(const FUBCharacterStat& BaseStat, const FUBCharacterStat& ModifierStat)
{
	HpBar->UpdateStat(BaseStat, ModifierStat);
	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UUBHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UUBHUDWidget::UpdateExpStat(const int32 InCurrentExp, const int32 InMaxExp)
{
	ExpBar->UpdateExpStat(InCurrentExp, InMaxExp);
}

void UUBHUDWidget::UpdateLevel(const int32 InNewCurrentLevel)
{
	ExpBar->UpdateLevel(InNewCurrentLevel);
}

void UUBHUDWidget::NativeConstruct()
{
	HpBar = Cast<UUBHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	ExpBar = Cast<UUBExpBarWidget>(GetWidgetFromName(TEXT("WidgetExpBar")));
	ensure(ExpBar);

	CharacterStat = Cast<UUBCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	IUBCharacterHUDInterface* HUDPawn = Cast<IUBCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
