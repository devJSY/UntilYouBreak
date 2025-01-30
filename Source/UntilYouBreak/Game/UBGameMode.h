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

	virtual void BeginPlay() override;

public:
	virtual void  OnEnemyDestroyed() override;
	virtual void  OnPlayerDead() override;
	virtual bool  IsGameCleared() override;
	virtual int32 GetCurrentStageNum() const override { return CurrentStageNum; };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 CurrentStageNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 RemainingEnemyCount;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1;
};
