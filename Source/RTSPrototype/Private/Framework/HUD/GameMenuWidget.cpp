// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/HUD/GameMenuWidget.h"

#include "Components/Button.h"
#include "Framework/HUD/BuildMenuWidget.h"


void UGameMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!Buildbutton)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildButton or Button subcomponent is null!"));
		return;
	}

	Buildbutton->OnClicked.AddDynamic(this, &UGameMenuWidget::ToggleBuildMenu);

	if (!BuildMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildMenuWidget is null!"));
		return;
	}
	if (BuildMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildMenuWidget is already build!"));
	}

	BuildMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	bBuildMenuOpen = false;

	BuildMenuWidget->OnBuildItemsSelectedEvent.AddDynamic(this, &UGameMenuWidget::OnBuildItemsSelected);
}

void UGameMenuWidget::ToggleBuildMenu()
{
	UE_LOG(LogTemp, Log, TEXT("ToggleBuildMenu called"));

	bBuildMenuOpen = !bBuildMenuOpen;
	if (BuildMenuWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("Setting BuildMenuWidget visibility to: %s"),
		       bBuildMenuOpen ? TEXT("Visible") : TEXT("Hidden"));

		BuildMenuWidget->SetVisibility(bBuildMenuOpen ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		if (bBuildMenuOpen)
		{
			BuildMenuWidget->DisplayBuildMenu();
			UE_LOG(LogTemp, Log, TEXT("Button clicked - calling DisplayBuildMenu"));
		}
		else
		{
			BuildMenuWidget->EndDisplayBuildMenu();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BuildMenuWidget is null in ToggleBuildMenu"));
	}
}

void UGameMenuWidget::OnBuildItemsSelected()
{
	ToggleBuildMenu();
}
