// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTDecorator_IsDead.h"
#include "BTDecorator_IsDead.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/UBCharacterAIInterface.h"

UBTDecorator_IsDead::UBTDecorator_IsDead()
{
}

bool UBTDecorator_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IUBCharacterAIInterface* AIPawn = Cast<IUBCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	return AIPawn->IsAIDead();
}
