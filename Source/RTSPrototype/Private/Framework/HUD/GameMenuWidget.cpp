// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/HUD/GameMenuWidget.h"

#include "Components/Button.h"
#include "Framework/HUD/BuildMenuWidget.h"


void UGameMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!BuildButton)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildButton or Button subcomponent is null!"));
		return;
	}

	BuildButton->OnClicked.AddDynamic(this, &UGameMenuWidget::ToggleBuildMenu);

	if (!BuildMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildMenuWidget is null!"));
		return;
	}

	BuildMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	bBuildMenuOpen = false;

	BuildMenuWidget->OnBuildItemsSelectedEvent.AddDynamic(this, &UGameMenuWidget::OnBuildItemsSelected);
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
