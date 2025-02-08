// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UBExpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameData/UBGameSingleton.h"

UUBExpBarWidget::UUBExpBarWidget(const FObjectInitializer& ObjectInitializer)
{
	bMaxLevel = false;
}

void UUBExpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ExpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbExpBar")));
	ensure(nullptr != ExpProgressBar);

	ExpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtExpStat")));
	ensure(ExpStat);

	CurrentLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtLevel")));
	ensure(CurrentLevel);
}

void UUBExpBarWidget::UpdateExpStat(const int32 InCurrentExp, const int32 InMaxExp)
{
	if (bMaxLevel)
		return;

	ensure(InMaxExp >= 0.0f);
	if (ExpProgressBar)
	{
		ExpProgressBar->SetPercent((float)InCurrentExp / InMaxExp);
	}

	if (ExpStat)
	{
		ExpStat->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), InCurrentExp, InMaxExp)));
	}
}

void UUBExpBarWidget::UpdateLevel(const int32 InNewCurrentLevel)
{
	if (bMaxLevel)
		return;

	if (CurrentLevel)
	{
		// ¸¸·¾
		if (InNewCurrentLevel == UUBGameSingleton::Get().CharacterMaxLevel)
		{
			CurrentLevel->SetText(FText::FromString(FString::Printf(TEXT("Lv : MAX"))));
			if (ExpProgressBar)
			{
				ExpProgressBar->SetPercent(100.0f);
			}

			if (ExpStat)
			{
				ExpStat->SetText(FText());
			}

			bMaxLevel = true;
		}
		else
		{
			CurrentLevel->SetText(FText::FromString(FString::Printf(TEXT("Lv : %d"), InNewCurrentLevel)));
		}
	}
}
