// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/HUD/GameMenuWidget.h"

#include "Components/Button.h"
#include "Framework/HUD/BuildMenuWidget.h"
#include "Framework/HUD/SimpleUIButton.h"

void UGameMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BuildButton)
	{
		BuildButton->Button->OnClicked.AddDynamic(this, &UGameMenuWidget::ToggleBuildMenu);
		if (BuildMenuWidget)
		{
			BuildMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			bBuildMenuOpen = false;
			BuildMenuWidget->OnBuildItemsSelectedEvent.AddDynamic(this, &UGameMenuWidget::OnBuildItemsSelected);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BuildMenuWidget is null in NativeOnInitialized!"));
		}
	}
}

void UGameMenuWidget::ToggleBuildMenu()
{
	bBuildMenuOpen = !bBuildMenuOpen;
	if (BuildMenuWidget)
	{
		BuildMenuWidget->SetVisibility(bBuildMenuOpen ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		bBuildMenuOpen ? BuildMenuWidget->DisplayBuildMenu() : BuildMenuWidget->EndDisplayBuildMenu();
	}
}

void UGameMenuWidget::OnBuildItemsSelected()
{
	ToggleBuildMenu();
}
