// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/UBCharacterStat.h"
#include "UBCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FUBCharacterStat& /*BaseStat*/, const FUBCharacterStat& /*ModifierStat*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnExpChangedDelegate, const int32 /*CurrentExp*/, const int32 /*MaxExp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelChangedDelegate, const int32 /*CurrentLevel*/);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNTILYOUBREAK_API UUBCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUBCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;

public:
	FOnHpZeroDelegate		OnHpZero;
	FOnHpChangedDelegate	OnHpChanged;
	FOnStatChangedDelegate	OnStatChanged;
	FOnExpChangedDelegate	OnExpChanged;
	FOnLevelChangedDelegate OnLevelChanged;

	void			  SetLevelStat(int32 InNewLevel);
	FORCEINLINE int32 GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE void  AddBaseStat(const FUBCharacterStat& InAddBaseStat)
	{
		float HealAmount = FMath::Clamp(InAddBaseStat.MaxHp - BaseStat.MaxHp, 0.0f, InAddBaseStat.MaxHp);
		BaseStat = BaseStat + InAddBaseStat;
		HealHp(HealAmount);
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}
	FORCEINLINE void SetBaseStat(const FUBCharacterStat& InBaseStat)
	{
		float HealAmount = FMath::Clamp(InBaseStat.MaxHp - BaseStat.MaxHp, 0.0f, InBaseStat.MaxHp);
		BaseStat = InBaseStat;
		HealHp(HealAmount);
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
		if (InHealAmount <= 0.f)
			return;

		CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp);
		OnHpChanged.Broadcast(CurrentHp);
	}
	void  SetHp(float NewHp);
	float ApplyDamage(float InDamage);

	void			  GainExp(int32 InAmountExp);
	FORCEINLINE int32 GetCurrentExp() const { return CurrentExp; }

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentExp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FUBCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FUBCharacterStat ModifierStat;
};
