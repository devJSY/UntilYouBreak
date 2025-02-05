// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interface/UBAnimationAttackInterface.h"
#include "Interface/UBCharacterWidgetInterface.h"
#include "Interface/UBCharacterItemInterface.h"
#include "GameData/UBCharacterStat.h"
#include "UBCharacterStatComponent.h"
#include "UBCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUBCharacter, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UUBItemData* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate)
		: ItemDelegate(InItemDelegate) {}
	FOnTakeItemDelegate ItemDelegate;
};

UCLASS(config = Game)
class AUBCharacterBase : public ACharacter, public IUBAnimationAttackInterface, public IUBCharacterWidgetInterface, public IUBCharacterItemInterface
{
	GENERATED_BODY()

public:
	AUBCharacterBase();

	virtual void PostInitializeComponents() override;

	// CharacterControl Section
protected:
	virtual void SetCharacterControlData(const class UUBCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UUBCharacterControlData*> CharacterControlManager;

	// Combo Action Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUBComboActionData> ComboActionData;

	void ProcessComboCommand();

	void		 ComboActionBegin();
	void		 ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyComboActionEnd();
	void		 SetComboCheckTimer();
	void		 ComboCheck();

	int32		 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool		 HasNextComboCommand = false;

	// Attack Hit Section
protected:
	virtual bool  AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void	 SetDead();
	void			 PlayDeadAnimation();
	FORCEINLINE bool IsDead() const { return Stat->GetCurrentHp() <= 0.f; }

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUBCharacterStatComponent> Stat;

	// UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UUBUserWidget* InUserWidget) override;

	// Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;
	class UUBWeaponItemData*				 WeaponItemData;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class UUBItemData* InItemData) override;
	
	virtual void EquipWeapon(class UUBItemData* InItemData);
	virtual void DrinkPotion(class UUBItemData* InItemData);
	virtual void ReadScroll(class UUBItemData* InItemData);

public:
	UFUNCTION()
	void DropWeapon(AActor* DestroyedActor);

	// Stat Section
public:
	int32 GetLevel();
	void  SetLevel(int32 InNewLevel);
	void  LevelUp();
	void  ApplyStat(const FUBCharacterStat& BaseStat, const FUBCharacterStat& ModifierStat);
};
