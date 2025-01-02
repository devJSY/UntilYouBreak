// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "UBCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS(config = Game)
class AUBCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AUBCharacterBase();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void SetCharacterControlData(const class UUBCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UUBCharacterControlData*> CharacterControlManager;
};
