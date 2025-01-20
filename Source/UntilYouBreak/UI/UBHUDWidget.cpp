// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UBHUDWidget.h"
#include "Interface/UBCharacterHUDInterface.h"
#include "UBHpBarWidget.h"
#include "UBCharacterStatWidget.h"

void UUBHUDWidget::UpdateStat(const FUBCharacterStat& BaseStat, const FUBCharacterStat& ModifierStat)
{
	FUBCharacterStat TotalStat = BaseStat + ModifierStat;
	HpBar->UpdateStat(BaseStat, ModifierStat);
	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UUBHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UUBHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UUBHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UUBCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	IUBCharacterHUDInterface* HUDPawn = Cast<IUBCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
