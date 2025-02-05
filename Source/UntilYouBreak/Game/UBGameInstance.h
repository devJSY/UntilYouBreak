// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UBGameInstance.generated.h"

/**
 *
 */
UCLASS()
class UNTILYOUBREAK_API UUBGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UUBGameInstance();

public:
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable, Category = SaveGame, Meta = (DisplayName = "OnDeleteStageSaveGameCpp"))
	void DeleteStageSaveGame();
};
