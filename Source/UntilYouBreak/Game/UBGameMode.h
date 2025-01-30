// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/UBGameInterface.h"
#include "UBGameMode.generated.h"

UCLASS(minimalapi)
class AUBGameMode : public AGameModeBase, public IUBGameInterface
{
	GENERATED_BODY()

public:
	AUBGameMode();

	virtual void PostInitializeComponents() override;

public:
	virtual void  OnEnemyDestroyed() override;
	virtual void  OnPlayerDead() override;
	virtual bool  IsGameCleared() override;
	virtual int32 GetCurrentStageLevel() const override { return CurrentStageLevel; };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 CurrentStageLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 RemainingEnemyCount;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1;

	// Save Game Section
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveGame)
	TObjectPtr<class UUBStageSaveGame> SaveGameInstance;
};
