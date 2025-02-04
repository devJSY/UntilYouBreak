// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/UBCharacterPlayer.h"
#include "UBCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UBCharacterControlData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "UI/UBHUDWidget.h"
#include "Interface/UBGameInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Animation/UBAnimInstance.h"

AUBCharacterPlayer::AUBCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;		// The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;								// Camera does not rotate relative to arm

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChangeControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UntilYouBreak/Input/Actions/IA_Change_Control.IA_Change_Control'"));
	if (InputActionChangeControlRef.Object)
	{
		ChangeControlAction = InputActionChangeControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UntilYouBreak/Input/Actions/IA_Jump.IA_Jump'"));
	if (InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UntilYouBreak/Input/Actions/IA_Quater_Move.IA_Quater_Move'"));
	if (InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraEffectAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/UntilYouBreak/Cursor/FX_Cursor.FX_Cursor'"));
	if (NiagaraEffectAsset.Object)
	{
		FXCursor = NiagaraEffectAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UntilYouBreak/Input/Actions/IA_Shoulder_Move.IA_Shoulder_Move'"));
	if (InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UntilYouBreak/Input/Actions/IA_Shoulder_Look.IA_Shoulder_Look'"));
	if (InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UntilYouBreak/Input/Actions/IA_Attack.IA_Attack'"));
	if (InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLevelUpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UntilYouBreak/Input/Actions/IA_LeevlUp.IA_LeevlUp'"));
	if (InputActionLevelUpRef.Object)
	{
		LevelUpAction = InputActionLevelUpRef.Object;
	}

	ShortPressThreshold = 0.3f;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	CurrentCharacterControlType = ECharacterControlType::Quater;
}

void AUBCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);
}

void AUBCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// ChangeControl
		EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AUBCharacterPlayer::ChangeCharacterControl);

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Quater
		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Started, this, &AUBCharacterPlayer::QuaterMoveOnInputStarted);
		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AUBCharacterPlayer::QuaterMoveOnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Completed, this, &AUBCharacterPlayer::QuaterMoveOnSetDestinationReleased);
		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Canceled, this, &AUBCharacterPlayer::QuaterMoveOnSetDestinationReleased);

		// Shoulder
		EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AUBCharacterPlayer::ShoulderMove);
		EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AUBCharacterPlayer::ShoulderLook);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AUBCharacterPlayer::Attack);

		// LevelUp
		EnhancedInputComponent->BindAction(LevelUpAction, ETriggerEvent::Triggered, this, &AUBCharacterBase::LevelUp);
	}
	else
	{
		UE_LOG(LogUBCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AUBCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUBCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUBCharacterPlayer::QuaterMoveOnInputStarted()
{
	if (APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
	{
		PlayerController->StopMovement();
	}
}

void AUBCharacterPlayer::QuaterMoveOnSetDestinationTriggered()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (nullptr == PlayerController)
		return;

	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool	   bHitSuccessful = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	FVector WorldDirection = (CachedDestination - GetActorLocation()).GetSafeNormal();
	AddMovementInput(WorldDirection, 1.0, false);
}

void AUBCharacterPlayer::QuaterMoveOnSetDestinationReleased()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (nullptr == PlayerController)
		return;

	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerController, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(PlayerController, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

void AUBCharacterPlayer::Attack()
{
	ProcessComboCommand();
}

void AUBCharacterPlayer::SetCharacterControlData(const UUBCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AUBCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		// 네비게이션 움직임 초기화
		if (APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
		{
			PlayerController->StopMovement();
		}

		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AUBCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	// ECharacterControlType에 해당하는 DataAsset 찾아오기
	UUBCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	// DataAsset 설정 값 적용
	SetCharacterControlData(NewCharacterControl);

	// InputMappingContext 변경
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;

	// 마우스 입력 뷰포트 설정
	if (PlayerController)
	{
		if (CurrentCharacterControlType == ECharacterControlType::Quater)
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);

			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(true);
		}
		else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
		{
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void AUBCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);

		IUBGameInterface* UBGameMode = Cast<IUBGameInterface>(GetWorld()->GetAuthGameMode());
		if (UBGameMode)
		{
			UBGameMode->OnPlayerDead();
		}
	}
}

void AUBCharacterPlayer::SetupHUDWidget(UUBHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());

		Stat->OnStatChanged.AddUObject(InHUDWidget, &UUBHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UUBHUDWidget::UpdateHpBar);
	}
}
