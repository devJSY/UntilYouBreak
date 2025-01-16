// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UBCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/UBCharacterAIInterface.h"
#include "UBNonPlayerCharacter.generated.h"

/**
 *
 */
UCLASS(config = UntilYouBreak)
class UNTILYOUBREAK_API AUBNonPlayerCharacter : public AUBCharacterBase, public IUBCharacterAIInterface
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

	// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;
};
