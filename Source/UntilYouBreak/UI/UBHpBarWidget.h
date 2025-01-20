// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UBUserWidget.h"
#include "GameData/UBCharacterStat.h"
#include "UBHpBarWidget.generated.h"

/**
 *
 */
UCLASS()
class UNTILYOUBREAK_API UUBHpBarWidget : public UUBUserWidget
{
	GENERATED_BODY()

public:
	UUBHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void	UpdateStat(const FUBCharacterStat& BaseStat, const FUBCharacterStat& ModifierStat);
	void	UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
