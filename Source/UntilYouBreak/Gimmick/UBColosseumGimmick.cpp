// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmick/UBColosseumGimmick.h"
#include "Character/UBNonPlayerCharacter.h"
#include "GameData/UBGameSingleton.h"
#include "Interface/UBGameInterface.h"
#include "GameFramework/GameModeBase.h"

AUBColosseumGimmick::AUBColosseumGimmick()
{
	EnemyClass = AUBNonPlayerCharacter::StaticClass();
}

// Called when the game starts or when spawned
void AUBColosseumGimmick::BeginPlay()
{
	Super::BeginPlay();

	OnEnemySpawn();
}

void AUBColosseumGimmick::OnEnemySpawn()
{
	IUBGameInterface* UBGameMode = Cast<IUBGameInterface>(GetWorld()->GetAuthGameMode());
	if (UBGameMode)
	{
		int32 CurrentStageNum = UBGameMode->GetCurrentStageNum();
		for (int32 i = 0; i < UUBGameSingleton::Get().GetStageLevel(CurrentStageNum).EnemyCount; ++i)
		{
			const FTransform	   SpawnTransform(GetActorLocation() + FVector::UpVector * 88.0f + FVector(FMath::FRandRange(-500.f, 500.f), FMath::FRandRange(-500, 500.f), 0.f));
			AUBNonPlayerCharacter* ABOpponentCharacter = GetWorld()->SpawnActorDeferred<AUBNonPlayerCharacter>(EnemyClass, SpawnTransform);
			if (ABOpponentCharacter)
			{
				ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AUBColosseumGimmick::OnEnemyDestroyed);
				ABOpponentCharacter->SetLevel(CurrentStageNum);
				ABOpponentCharacter->FinishSpawning(SpawnTransform);
			}
		}
	}
}

void AUBColosseumGimmick::OnEnemyDestroyed(AActor* DestroyedActor)
{
	IUBGameInterface* UBGameMode = Cast<IUBGameInterface>(GetWorld()->GetAuthGameMode());
	if (UBGameMode)
	{
		UBGameMode->OnEnemyDestroyed();
	}
}
