// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UBWidgetComponent.h"
#include "UBWidgetComponent.h"
#include "UBUserWidget.h"

void UUBWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UUBUserWidget* UBUserWidget = Cast<UUBUserWidget>(GetWidget());
	if (UBUserWidget)
	{
		UBUserWidget->SetOwningActor(GetOwner());
	}
}