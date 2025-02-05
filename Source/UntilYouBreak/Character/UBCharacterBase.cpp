// Copyright Epic Games, Inc. All Rights Reserved.

#include "UBCharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "UBCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "UBComboActionData.h"
#include "Physics/UBCollision.h"
#include "Engine/DamageEvents.h"
#include "UI/UBWidgetComponent.h"
#include "UI/UBHpBarWidget.h"
#include "UBCharacterStatComponent.h"
#include "Item/UBItemData.h"
#include "Item/UBWeaponItemData.h"
#include "Item/UBDropItem.h"

DEFINE_LOG_CATEGORY(LogUBCharacter);

//////////////////////////////////////////////////////////////////////////
// AUBCharacterBase

AUBCharacterBase::AUBCharacterBase()
{
	// Pawn
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Cpasule
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_UBCAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;			 // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/UntilYouBreak/Characters/Meshes/Y-Bot.Y-Bot'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/UntilYouBreak/Characters/Animations/ABP_UBCharacter.ABP_UBCharacter_C'"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// CharacterControlData
	static ConstructorHelpers::FObjectFinder<UUBCharacterControlData> ShoulderDataRef(TEXT("/Script/UntilYouBreak.UBCharacterControlData'/Game/UntilYouBreak/CharacterControl/UBC_Shoulder.UBC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UUBCharacterControlData> QuaterDataRef(TEXT("/Script/UntilYouBreak.UBCharacterControlData'/Game/UntilYouBreak/CharacterControl/UBC_Quater.UBC_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/UntilYouBreak/Characters/Animations/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UUBComboActionData> ComboActionDataRef(TEXT("/Script/UntilYouBreak.UBComboActionData'/Game/UntilYouBreak/CharacterControl/UBC_ComboAction.UBC_ComboAction'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/UntilYouBreak/Characters/Animations/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// Stat Component
	Stat = CreateDefaultSubobject<UUBCharacterStatComponent>(TEXT("Stat"));

	// Widget Component
	HpBar = CreateDefaultSubobject<UUBWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UntilYouBreak/UI/WBP_HpBarWidget.WBP_HpBarWidget_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Item Actions
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AUBCharacterBase::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AUBCharacterBase::DrinkPotion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AUBCharacterBase::ReadScroll)));

	// Weapon Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

void AUBCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AUBCharacterBase::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AUBCharacterBase::ApplyStat);
}

void AUBCharacterBase::SetCharacterControlData(const UUBCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AUBCharacterBase::ProcessComboCommand()
{
	if (0 == CurrentCombo)
	{
		// 땅에 붙어있지 않는 경우 공격 수행 X
		if (!GetCharacterMovement()->IsMovingOnGround())
			return;

		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void AUBCharacterBase::ComboActionBegin()
{
	// Combo Status
	CurrentCombo = 1;

	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	const float	   AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	ensure(AnimInstance != nullptr);
	ensure(ComboActionMontage != nullptr);
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	// End 델리게이트 등록
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AUBCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	// 타이머 등록
	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AUBCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	NotifyComboActionEnd();
}

void AUBCharacterBase::NotifyComboActionEnd()
{
}

void AUBCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	float		ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		// ComboEffectiveTime 시간 후에 ComboCheck() 함수 호출 타이머 등록
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AUBCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void AUBCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();

	// 다음 콤보 액션 재생
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

bool AUBCharacterBase::AttackHitCheck()
{
	FHitResult			  OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float	  AttackRange = Stat->GetTotalStat().AttackRange;
	const float	  AttackRadius = Stat->GetTotalStat().AttackRadius;
	const float	  AttackDamage = Stat->GetTotalStat().Attack;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_UBACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float	CapsuleHalfHeight = AttackRange * 0.5f;
	FColor	DrawColor = HitDetected ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif

	return HitDetected;
}

#include "UBCharacterPlayer.h"

float AUBCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// Test
	AUBCharacterPlayer* Player = Cast<AUBCharacterPlayer>(this);
	if (!Player)
	{
		Stat->ApplyDamage(DamageAmount);
	}

	return DamageAmount;
}

void AUBCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
}

void AUBCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	ensure(AnimInstance != nullptr);
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void AUBCharacterBase::SetupCharacterWidget(UUBUserWidget* InUserWidget)
{
	UUBHpBarWidget* HpBarWidget = Cast<UUBHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UUBHpBarWidget::UpdateHpBar);
		Stat->OnStatChanged.AddUObject(HpBarWidget, &UUBHpBarWidget::UpdateStat);
	}
}

void AUBCharacterBase::TakeItem(UUBItemData* InItemData)
{
	if (InItemData)
	{
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void AUBCharacterBase::DrinkPotion(UUBItemData* InItemData)
{
	UE_LOG(LogUBCharacter, Log, TEXT("Drink Potion"));
}

void AUBCharacterBase::EquipWeapon(UUBItemData* InItemData)
{
	UUBWeaponItemData* InWeaponItemData = Cast<UUBWeaponItemData>(InItemData);
	if (InWeaponItemData)
	{
		// 이미 무기를 들고있는 경우 내려놓는다.
		if (nullptr != WeaponItemData)
		{
			DropWeapon(this);
		}

		WeaponItemData = InWeaponItemData;

		// 무기 획득
		if (WeaponItemData->WeaponMesh.IsPending())
		{
			WeaponItemData->WeaponMesh.LoadSynchronous();
		}
		Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
		Stat->SetModifierStat(WeaponItemData->ModifierStat);
	}
}

void AUBCharacterBase::ReadScroll(UUBItemData* InItemData)
{
	UE_LOG(LogUBCharacter, Log, TEXT("Read Scroll"));
}

void AUBCharacterBase::DropWeapon(AActor* DestroyedActor)
{
	if (WeaponItemData)
	{
		const FTransform SpawnTransform(GetActorLocation());
		AUBDropItem*	 DropItem = GetWorld()->SpawnActorDeferred<AUBDropItem>(AUBDropItem::StaticClass(), SpawnTransform);
		if (DropItem)
		{
			DropItem->SetItem(WeaponItemData);
			DropItem->FinishSpawning(SpawnTransform);
		}

		WeaponItemData = nullptr;
	}
}

int32 AUBCharacterBase::GetLevel()
{
	return Stat->GetCurrentLevel();
}

void AUBCharacterBase::SetLevel(int32 InNewLevel)
{
	Stat->SetLevelStat(InNewLevel);
}

void AUBCharacterBase::LevelUp()
{
	SetLevel(GetLevel() + 1);
}

void AUBCharacterBase::ApplyStat(const FUBCharacterStat& BaseStat, const FUBCharacterStat& ModifierStat)
{
	float MovementSpeed = (BaseStat + ModifierStat).MovementSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}
