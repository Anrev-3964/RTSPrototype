// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/HUD/SimpleUIButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USimpleUIButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (ButtonTextBlock)
	{
		ButtonTextBlock->SetText(FText::FromString(ButtonText));
	}
}
