// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UBCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "UBNonPlayerCharacter.generated.h"

/**
 *
 */
UCLASS(config = UntilYouBreak)
class UNTILYOUBREAK_API AUBNonPlayerCharacter : public AUBCharacterBase
{
	GENERATED_BODY()

public:
	AUBNonPlayerCharacter();

protected:
	virtual void PostInitializeComponents() override;

protected:
	virtual void SetDead() override;
	void		 NPCMeshLoadCompleted();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;
};
