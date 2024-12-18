// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/HUD/BuildMenuWidget.h"

#include "Buildings/BuildComponent.h"
#include "Components/Border.h"
#include "Components/ListView.h"
#include "Core/SPlayerController.h"
#include "Engine/AssetManager.h"
#include "Framework/DataAssets/BuildItemDataAsset.h"
#include "Kismet/GameplayStatics.h"

void UBuildMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MenuBorder)
	{
		MenuBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBuildMenuWidget::DisplayBuildMenu()
{
	if (!BuildItemsList)
	{
		return;
	}

	verify((AssetManager = UAssetManager::GetIfInitialized()) != nullptr);
	verify((SPlayer = Cast<ASPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) != nullptr);
	verify((BuildComponent = UBuildComponent::FindBuildComponent(SPlayer)) != nullptr);

	BuildItemsList->ClearListItems();

	//populate build menu
	const TArray<FPrimaryAssetId> BuildItemDataAssets = BuildComponent->GetBuildData();
	for (int i = 0; i < BuildItemDataAssets.Num(); i++)
	{
		if (UBuildItemDataAsset* BuildData = Cast<UBuildItemDataAsset>(
			AssetManager->GetPrimaryAssetObject(BuildItemDataAssets[i])))
		{
			BuildItemsList->AddItem(BuildData);
		}
	}

	//Show build menu
	BuildItemsList->SetVisibility(ESlateVisibility::Visible);
	MenuBorder->SetVisibility(ESlateVisibility::Visible);

	if (BuildComponent && !BuildComponent->OnBuildModeEnterEvent.IsBound())
	{
		BuildComponent->OnBuildModeEnterEvent.AddDynamic(this, &UBuildMenuWidget::OnBuildModeEnteredEvent);
	}
}

void UBuildMenuWidget::EndDisplayBuildMenu() const
{
	BuildItemsList->ClearListItems();
	BuildItemsList->SetVisibility(ESlateVisibility::Hidden);
	MenuBorder->SetVisibility(ESlateVisibility::Hidden);

	if (BuildComponent && !BuildComponent->OnBuildModeEnterEvent.IsBound())
	{
		BuildComponent->OnBuildModeEnterEvent.RemoveDynamic(this, &UBuildMenuWidget::OnBuildModeEnteredEvent);
	}
}

void UBuildMenuWidget::OnBuildModeEnteredEvent()
{
	EndDisplayBuildMenu();
}
