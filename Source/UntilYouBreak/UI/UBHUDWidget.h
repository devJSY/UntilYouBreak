// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/UBCharacterStat.h"
#include "UBHUDWidget.generated.h"

/**
 *
 */
UCLASS()
class UNTILYOUBREAK_API UUBHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateStat(const FUBCharacterStat& BaseStat, const FUBCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UUBHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UUBCharacterStatWidget> CharacterStat;
};
