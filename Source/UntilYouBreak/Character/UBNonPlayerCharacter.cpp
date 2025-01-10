// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/UBNonPlayerCharacter.h"
#include "UBNonPlayerCharacter.h"

void AUBNonPlayerCharacter::SetDead()
{
	Super::SetDead();

	const float	 DeadEventDelayTime = 5.0f;
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([&]() {
		Destroy();
	}),
		DeadEventDelayTime, false);
}