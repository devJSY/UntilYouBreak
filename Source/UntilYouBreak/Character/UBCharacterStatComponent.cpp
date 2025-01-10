// Fill out your copyright notice in the Description page of Project Settings.

#include "UBCharacterStatComponent.h"

UUBCharacterStatComponent::UUBCharacterStatComponent()
{
	MaxHp = 200.0f;
	CurrentHp = MaxHp;
}

void UUBCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
}

float UUBCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= 0.0f)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UUBCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}


