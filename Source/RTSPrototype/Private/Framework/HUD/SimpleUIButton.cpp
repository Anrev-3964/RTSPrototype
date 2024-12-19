// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/HUD/SimpleUIButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USimpleUIButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (ButtonTextBlock)
	{
		// Use the existing text if it is set, otherwise use the default C++ value.
		if (ButtonText.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("ButtonText is empty. Setting default text."));
			ButtonText = FText::FromString(TEXT("Default Button"));
		}

		ButtonTextBlock->SetText(ButtonText);
		UE_LOG(LogTemp, Log, TEXT("ButtonTextBlock updated with text: %s"), *ButtonText.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ButtonTextBlock is null in SimpleUIButton!"));
	}
}
