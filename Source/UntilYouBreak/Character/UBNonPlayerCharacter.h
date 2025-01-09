// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UBCharacterBase.h"
#include "UBNonPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UNTILYOUBREAK_API AUBNonPlayerCharacter : public AUBCharacterBase
{
	GENERATED_BODY()

protected:
	virtual void SetDead() override;
};
