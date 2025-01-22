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

	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) override;
	virtual void OnPlayerDead() override;
	virtual bool IsGameCleared() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 ClearScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	int32 CurrentScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1;
};



