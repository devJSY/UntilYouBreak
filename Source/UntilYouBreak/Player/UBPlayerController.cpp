// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UBPlayerController.h"

void AUBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� ���콺 �Է��� ����Ʈ���η� ����
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
