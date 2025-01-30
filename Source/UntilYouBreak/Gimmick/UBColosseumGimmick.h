// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UBColosseumGimmick.generated.h"

UCLASS()
class UNTILYOUBREAK_API AUBColosseumGimmick : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUBColosseumGimmick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = Enemy, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUBNonPlayerCharacter> EnemyClass;

	void									 OnEnemySpawn();

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
};
