// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/BuildComponent.h"

#include "Engine/AssetManager.h"
#include "Framework/DataAssets/BuildItemDataAsset.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBuildComponent::UBuildComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBuildComponent::BeginPlay()
{
	Super::BeginPlay();

	verify((SPlayer = Cast<ASPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) != nullptr);
}

void UBuildComponent::UpdatePlacementStatus()
{
}

void UBuildComponent::OnBuildDataLoaded(TArray<FPrimaryAssetId> BuildAssetsIds)
{
	if (!SPlayer)
	{
		return;
	}
	for (int i = 0; i < BuildAssetsIds.Num(); i++)
	{
		if (const UBuildItemDataAsset* BuildDataAsset = Cast<UBuildItemDataAsset>(
			AssetManager->GetPrimaryAssetObject(BuildAssetsIds[i])))
		{
			if (BuildDataAsset->BuildAssetFilter == SPlayer->GetBuildFilter())
			{
				BuildItemsData.Add(BuildAssetsIds[i]);
			}
		}
	}
}

void UBuildComponent::EnterBuildPlacementMode(UBuildItemDataAsset* BuildItemData)
{
}

void UBuildComponent::LoadBuildData()
{
	verify((AssetManager = UAssetManager::GetIfInitialized()) != nullptr);

	const FPrimaryAssetType AssetType("Build Data");
	TArray<FPrimaryAssetId> BuildDataAssets;
	AssetManager->GetPrimaryAssetIdList(AssetType, BuildDataAssets);

	if (BuildDataAssets.Num() > 0)
	{
		const TArray<FName> Bundles;
		const FStreamableDelegate FormationDataLoadedDelegate =
			FStreamableDelegate::CreateUObject(this, &UBuildComponent::OnBuildDataLoaded, BuildDataAssets);
		AssetManager->LoadPrimaryAssets(BuildDataAssets, Bundles, FormationDataLoadedDelegate);
	}
}


// Called every frame
void UBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
