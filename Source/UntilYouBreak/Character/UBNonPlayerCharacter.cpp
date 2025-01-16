// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/UBNonPlayerCharacter.h"
#include "UBNonPlayerCharacter.h"
#include "Engine/AssetManager.h"

AUBNonPlayerCharacter::AUBNonPlayerCharacter()
{
	GetMesh()->SetHiddenInGame(true);
}

void AUBNonPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(NPCMeshes.Num() > 0);
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AUBNonPlayerCharacter::NPCMeshLoadCompleted));
}

void AUBNonPlayerCharacter::SetDead()
{
	Super::SetDead();

	const float	 DeadEventDelayTime = 5.0f;
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([&]() {
		Destroy();
	}),
		DeadEventDelayTime, false);
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
