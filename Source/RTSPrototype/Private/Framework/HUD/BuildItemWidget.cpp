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
		PlaceBuildButton->Button->OnClicked.AddDynamic(
			this, &UBuildItemWidget::UBuildItemWidget::OnPlaceBuildItemSelected);
	}

	verify((SPlayer = Cast<ASPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) != nullptr);
	BuildComponent = UBuildComponent::FindBuildComponent(SPlayer);
}

void UBuildItemWidget::OnPlaceBuildItemSelected()
{
	if (!BuildComponent)
	{
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
		TitleText->SetText(Data->TitleText);
		Image->SetBrushFromTexture(Data->Image.LoadSynchronous());
	}
}
