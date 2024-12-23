// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/HUD/BuildItemWidget.h"

#include "Buildings/BuildComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Framework/DataAssets/BuildItemDataAsset.h"
#include "Kismet/GameplayStatics.h"

void UBuildItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (PlaceBuildButton)
	{
		UE_LOG(LogTemp, Log, TEXT("PlaceBuildButton is valid"));
		PlaceBuildButton->OnClicked.AddDynamic(
			this, &UBuildItemWidget::OnPlaceBuildItemSelected);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlaceBuildButton is null"));
	}

	verify((SPlayer = Cast<ASPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) != nullptr);
	BuildComponent = UBuildComponent::FindBuildComponent(SPlayer);
}

void UBuildItemWidget::OnPlaceBuildItemSelected()
{
	UE_LOG(LogTemp, Log, TEXT("OnPlaceBuildItemSelected called"));
	if (!BuildComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildComponent is null"));
		return;
	}

	if (!Data)
	{
		UE_LOG(LogTemp, Error, TEXT("Data is null in OnPlaceBuildItemSelected"));
		return;
	}

	BuildComponent->EnterBuildPlacementMode(Data);
}

void UBuildItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	Data = Cast<UBuildItemDataAsset>(ListItemObject);
	if (Data)
	{
		UE_LOG(LogTemp, Error, TEXT("Found data"));
		TitleText->SetText(Data->TitleText);
		Image->SetBrushFromTexture(Data->Image.LoadSynchronous());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get UBuildItemDataAsset"));
	}
}
