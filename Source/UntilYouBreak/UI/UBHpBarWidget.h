// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UBUserWidget.h"
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
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void			 UpdateHpBar(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
};
