// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "UBCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AUBCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AUBCharacterBase();

protected:
	virtual void BeginPlay() override;
};

