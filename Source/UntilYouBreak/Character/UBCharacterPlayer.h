// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UBCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/UBCharacterHUDInterface.h"
#include "UBCharacterPlayer.generated.h"

/**
 *
 */
UCLASS()
class UNTILYOUBREAK_API AUBCharacterPlayer : public AUBCharacterBase, public IUBCharacterHUDInterface
{
	GENERATED_BODY()

public:
	AUBCharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Camera Section
protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UNiagaraSystem> FXCursor;

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32	bMoveToMouseCursor : 1;
	FVector CachedDestination;
	float	FollowTime; // For how long it has been pressed

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> GodModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> PickupAction;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMoveOnInputStarted();
	void QuaterMoveOnSetDestinationTriggered();
	void QuaterMoveOnSetDestinationReleased();

	ECharacterControlType CurrentCharacterControlType;

	UFUNCTION(BlueprintCallable, Category = CharacterControl, Meta = (DisplayName = "OnSetCharacterControlInputModeCpp"))
	void SetCharacterControlInputMode();

	void Attack();

	void Pickup();

	// Character Control Section
protected:
	virtual void SetCharacterControlData(const class UUBCharacterControlData* CharacterControlData) override;

	void ChangeCharacterControl();

	void SetCharacterControl(ECharacterControlType NewCharacterControlType);

	// Dead Section
protected:
	virtual void SetDead() override;

	// UI Section
protected:
	virtual void SetupHUDWidget(class UUBHUDWidget* InHUDWidget) override;
};
