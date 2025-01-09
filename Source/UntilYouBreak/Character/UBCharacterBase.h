// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interface/UBAnimationAttackInterface.h"
#include "UBCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS(config = Game)
class AUBCharacterBase : public ACharacter, public IUBAnimationAttackInterface
{
	GENERATED_BODY()

public:
	AUBCharacterBase();

protected:
	virtual void BeginPlay() override;

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

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer();
	void ComboCheck();

	int32		 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool		 HasNextComboCommand = false;

	// Attack Hit Section
protected:
	virtual void  AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void		 PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;
};
