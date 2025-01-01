// Fill out your copyright notice in the Description page of Project Settings.

#include "Prop/UBStairs.h"
#include "Components/PointLightComponent.h"

// Sets default values
AUBStairs::AUBStairs()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));

	RootComponent = Body;
	PointLight->SetupAttachment(Body);
	PointLight->SetRelativeLocation(FVector(0.0f, 0.0f, 132.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_Stairs.SM_Stairs'"));
	if (BodyMeshRef.Object)
	{
		Body->SetStaticMesh(BodyMeshRef.Object);
	}
}

// Called when the game starts or when spawned
void AUBStairs::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(LightTimerHandle, FTimerDelegate::CreateLambda([&]() {

		FLinearColor RandColor(FMath::FRandRange(0.f, 1.f), FMath::FRandRange(0.f, 1.f), FMath::FRandRange(0.f, 1.f));

		PointLight->SetLightColor(RandColor);
	}),
		1.0f, true);
}

// Called every frame
void AUBStairs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
