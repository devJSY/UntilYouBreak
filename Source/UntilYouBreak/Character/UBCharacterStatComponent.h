// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/UBCharacterStat.h"
#include "UBCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FUBCharacterStat& /*BaseStat*/, const FUBCharacterStat& /*ModifierStat*/);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNTILYOUBREAK_API UUBCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUBCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;

public:
	FOnHpZeroDelegate	   OnHpZero;
	FOnHpChangedDelegate   OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	void			  SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE void  AddBaseStat(const FUBCharacterStat& InAddBaseStat)
	{
		BaseStat = BaseStat + InAddBaseStat;
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}
	FORCEINLINE void SetBaseStat(const FUBCharacterStat& InBaseStat)
	{
		BaseStat = InBaseStat;
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}
	FORCEINLINE void SetModifierStat(const FUBCharacterStat& InModifierStat)
	{
		ModifierStat = InModifierStat;
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}

	FORCEINLINE const FUBCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FUBCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE FUBCharacterStat		GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE float					GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE void					HealHp(float InHealAmount)
	{
		CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp);
		OnHpChanged.Broadcast(CurrentHp);
	}
	float			  ApplyDamage(float InDamage);

protected:
	void SetHp(float NewHp);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FUBCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FUBCharacterStat ModifierStat;
};
