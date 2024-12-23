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
	verify((AssetManager = UAssetManager::GetIfInitialized()) != nullptr);
	verify((SPlayer = Cast<ASPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) != nullptr);
	verify((BuildComponent = UBuildComponent::FindBuildComponent(SPlayer)) != nullptr);
	
    if (!BuildItemsList)
    {
        UE_LOG(LogTemp, Error, TEXT("BuildItemsList is NULL"));
        return;
    }

	if (BuildComponent)
	{
		BuildComponent->LoadBuildData(); // Ensure data is loaded
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BuildComponent is NULL. Cannot retrieve BuildData."));
		return;
	}

	const TArray<FPrimaryAssetId> BuildItemDataAssets = BuildComponent->GetBuildData();
	if (BuildItemDataAssets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No build data found. Did LoadBuildData run?"));
		return;
	}

    

    BuildItemsList->ClearListItems();

    //const TArray<FPrimaryAssetId> BuildItemDataAssets = BuildComponent->GetBuildData();
    if (BuildItemDataAssets.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("BuildItemDataAssets is empty."));
    }

    for (int i = 0; i < BuildItemDataAssets.Num(); i++)
    {
        if (UBuildItemDataAsset* BuildData = Cast<UBuildItemDataAsset>(
            AssetManager->GetPrimaryAssetObject(BuildItemDataAssets[i])))
        {
            BuildItemsList->AddItem(BuildData);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load BuildItemDataAsset: %s"), *BuildItemDataAssets[i].ToString());
        }
    }

    BuildItemsList->SetVisibility(ESlateVisibility::Visible);
    MenuBorder->SetVisibility(ESlateVisibility::Visible);

    if (BuildComponent && !BuildComponent->OnBuildModeEnterEvent.IsBound())
    {
        BuildComponent->OnBuildModeEnterEvent.AddDynamic(this, &UBuildMenuWidget::OnBuildModeEnteredEvent);
    }

    UE_LOG(LogTemp, Log, TEXT("DisplayBuildMenu completed successfully."));
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
