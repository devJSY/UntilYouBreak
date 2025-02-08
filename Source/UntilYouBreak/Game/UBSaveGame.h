// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Character/UBCharacterStatComponent.h"
#include "UBSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UNTILYOUBREAK_API UUBSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UUBSaveGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 RetryCount;
};
