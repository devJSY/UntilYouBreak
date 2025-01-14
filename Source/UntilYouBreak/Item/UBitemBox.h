// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UBitemBox.generated.h"

UCLASS()
class UNTILYOUBREAK_API AUBItemBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUBItemBox();

	virtual void PostInitializeComponents() override;

	FORCEINLINE class UBoxComponent* GetTrigger() const { return Trigger; }

protected:
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> Effect;

	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UUBItemData> Item;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);
};
