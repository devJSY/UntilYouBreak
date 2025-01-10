// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UBHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/UBCharacterWidgetInterface.h"

UUBHpBarWidget::UUBHpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UUBHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(nullptr != HpProgressBar);

	IUBCharacterWidgetInterface* CharacterWidget = Cast<IUBCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UUBHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
