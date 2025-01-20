// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/UBPlayerController.h"
#include "UI/UBHUDWidget.h"

AUBPlayerController::AUBPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUBHUDWidget> UBHUDWidgetRef(TEXT("/Game/UntilYouBreak/UI/WBP_UBHUD.WBP_UBHUD_C"));
	if (UBHUDWidgetRef.Class)
	{
		UBHUDWidgetClass = UBHUDWidgetRef.Class;
	}
}

void AUBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UBHUDWidget = CreateWidget<UUBHUDWidget>(this, UBHUDWidgetClass);
	if (UBHUDWidget)
	{
		UBHUDWidget->AddToViewport();
	}
}
