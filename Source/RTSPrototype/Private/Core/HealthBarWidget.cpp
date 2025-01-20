// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HealthBarWidget.h"

void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHealthBarWidget::SetBarPercent(float const Value)
{
	HealthBar->SetPercent(Value);
}
