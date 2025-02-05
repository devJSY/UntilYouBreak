// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/UBDropItem.h"
#include "Components/BoxComponent.h"
#include "Physics/UBCollision.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interface/UBCharacterItemInterface.h"
#include "Engine/AssetManager.h"
#include "UBWeaponItemData.h"
#include "Components/WidgetComponent.h"

// Sets default values
AUBDropItem::AUBDropItem()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_UBTRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AUBDropItem::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AUBDropItem::OnOverlapEnd);

	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/UntilYouBreak/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		Effect->bAutoActivate = false;
	}

	// Widget Component
	PickupKeyWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	PickupKeyWidget->SetupAttachment(Mesh);
	PickupKeyWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> PickupKeyWidgetRef(TEXT("/Game/UntilYouBreak/UI/WBP_PickupKey.WBP_PickupKey_C"));
	if (PickupKeyWidgetRef.Class)
	{
		PickupKeyWidget->SetWidgetClass(PickupKeyWidgetRef.Class);
		PickupKeyWidget->SetWidgetSpace(EWidgetSpace::Screen);
		PickupKeyWidget->SetDrawSize(FVector2D(50.0f, 50.0f));
		PickupKeyWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PickupKeyWidget->SetVisibility(false);
	}
}

void AUBDropItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetItem(Item);
}

void AUBDropItem::SetItem(UUBWeaponItemData* InItem)
{
	Item = InItem;

	if (Item)
	{
		if (Item->WeaponMesh.IsPending())
		{
			Item->WeaponMesh.LoadSynchronous();
		}
		Mesh->SetSkeletalMesh(Item->WeaponMesh.Get());
	}
}

void AUBDropItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	IUBCharacterItemInterface* OverlappingPawn = Cast<IUBCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		PickupKeyWidget->SetVisibility(true);
	}
}

void AUBDropItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IUBCharacterItemInterface* OverlappingPawn = Cast<IUBCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		PickupKeyWidget->SetVisibility(false);
	}
}

void AUBDropItem::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}

void AUBDropItem::PickupItem(AActor* OtherActor)
{
	if (nullptr == Item)
	{
		Destroy();
		return;
	}

	IUBCharacterItemInterface* OverlappingPawn = Cast<IUBCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}

	Effect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	Effect->OnSystemFinished.AddDynamic(this, &AUBDropItem::OnEffectFinished);
}
