// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UBUserWidget.h"
#include "UBExpBarWidget.generated.h"

/**
 *
 */
UCLASS()
class UNTILYOUBREAK_API UUBExpBarWidget : public UUBUserWidget
{
	GENERATED_BODY()

public:
	UUBExpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void	UpdateExpStat(const int32 InCurrentExp, const int32 InMaxExp);
	void	UpdateLevel(const int32 InNewCurrentLevel);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> ExpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> ExpStat;

	UPROPERTY()
	TObjectPtr<class UTextBlock> CurrentLevel;

	bool bMaxLevel;
};
