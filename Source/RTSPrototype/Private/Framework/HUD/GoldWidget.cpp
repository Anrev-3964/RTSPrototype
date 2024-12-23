// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/HUD/GoldWidget.h"
#include "Components/TextBlock.h"
#include "Framework/RTSPlayerState.h"

void UGoldWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerState = Cast<ARTSPlayerState>(PlayerController->PlayerState);

		if (PlayerState)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerState successfully assigned with Gold: %d"), PlayerState->GetGold());
			PlayerState->OnGoldChanged.AddDynamic(this, &UGoldWidget::SetGoldText);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerState assignment failed!"));
		}
	}

	if (PlayerState && GoldText)
	{
		GoldText->SetText(FText::AsNumber(PlayerState->GetGold()));
	}
}

void UGoldWidget::SetGoldText()
{
	GoldText->SetText(FText::AsNumber(PlayerState->GetGold()));
}
