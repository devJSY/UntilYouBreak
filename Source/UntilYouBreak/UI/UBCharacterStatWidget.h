// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/UBCharacterStat.h"
#include "UBCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNTILYOUBREAK_API UUBCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FUBCharacterStat& BaseStat, const FUBCharacterStat& ModifierStat);

private:
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookup;

	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierLookup;
	
};
