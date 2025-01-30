// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UBStageSaveGame.generated.h"

/**
 *
 */
UCLASS()
class UNTILYOUBREAK_API UUBStageSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UUBStageSaveGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 StageLevel;
};
