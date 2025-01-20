// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UBPlayerController.generated.h"

/**
 *
 */
UCLASS()
class UNTILYOUBREAK_API AUBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUBPlayerController();

protected:
	virtual void BeginPlay() override;

	// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UUBHUDWidget> UBHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UUBHUDWidget> UBHUDWidget;
};
