// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UBCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangeDelegate, float /*CurrentHp*/);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNTILYOUBREAK_API UUBCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUBCharacterStatComponent();

protected:
	virtual void BeginPlay() override;

public:
	FOnHpZeroDelegate	OnHpZero;
	FOnHpChangeDelegate OnHpChanged;

	FORCEINLINE float GetMaxHp() const { return MaxHp; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	float			  ApplyDamage(float InDamage);

protected:
	void SetHp(float NewHp);

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;
};
