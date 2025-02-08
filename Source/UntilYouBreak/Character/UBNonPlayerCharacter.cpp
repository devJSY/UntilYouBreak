// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/UBNonPlayerCharacter.h"
#include "UBNonPlayerCharacter.h"
#include "AI/UBAIController.h"
#include "Character/UBCharacterStatComponent.h"
#include "Item/UBWeaponItemData.h"
#include "GameData/UBGameSingleton.h"
#include "Engine/AssetManager.h"

AUBNonPlayerCharacter::AUBNonPlayerCharacter()
{
	GetMesh()->SetHiddenInGame(true);

	AIControllerClass = AUBAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AUBNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 사망 시 드랍 아이템 생성
	OnDestroyed.AddDynamic(this, &AUBCharacterBase::DropWeapon);
}

void AUBNonPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Mesh 설정
	ensure(NPCMeshes.Num() > 0);
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AUBNonPlayerCharacter::NPCMeshLoadCompleted));
}

float AUBNonPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Stat->GetCurrentHp() <= 0.0f)
	{
		AUBCharacterBase* Killer = Cast<AUBCharacterBase>(DamageCauser);
		if (Killer)
		{
			Killer->GainExp(UUBGameSingleton::Get().GetCharacterExp(Stat->GetCurrentLevel()).RewardExp);
		}
	}

	return DamageAmount;
}

void AUBNonPlayerCharacter::SetDead()
{
	Super::SetDead();

	const float	 DeadEventDelayTime = 3.0f;
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([&]() {
		Destroy();
	}),
		DeadEventDelayTime, false);

	// Stop AI
	AUBAIController* AIController = Cast<AUBAIController>(GetController());
	if (IsValid(AIController))
	{
		AIController->StopAI();
	}
}

void AUBNonPlayerCharacter::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	NPCMeshHandle->ReleaseHandle();
}

float AUBNonPlayerCharacter::GetAIPatrolRadius()
{
	return 800.0f;
}

float AUBNonPlayerCharacter::GetAIDetectRange()
{
	return 400.0f;
}

float AUBNonPlayerCharacter::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetTotalStat().AttackRadius * 2;
}

float AUBNonPlayerCharacter::GetAITurnSpeed()
{
	return 2.0f;
}

void AUBNonPlayerCharacter::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AUBNonPlayerCharacter::AttackByAI()
{
	if (IsDead())
		return;

	ProcessComboCommand();
}

void AUBNonPlayerCharacter::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}

bool AUBNonPlayerCharacter::AttackHitCheck()
{
	bool HitResult = Super::AttackHitCheck();

	if (HitResult)
	{
		ProcessComboCommand();
	}

	return HitResult;
}
