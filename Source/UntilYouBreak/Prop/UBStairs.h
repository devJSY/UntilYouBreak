// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UBStairs.generated.h"

UCLASS()
class UNTILYOUBREAK_API AUBStairs : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUBStairs();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light)
	TObjectPtr<class UPointLightComponent> PointLight;

	FTimerHandle LightTimerHandle;
};