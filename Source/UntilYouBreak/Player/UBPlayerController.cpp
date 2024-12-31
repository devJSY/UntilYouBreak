// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UBPlayerController.h"

void AUBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 마우스 입력이 뷰포트내부로 설정
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
