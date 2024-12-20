// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/BuildComponent.h"

#include "Buildings/Buildable.h"
#include "Buildings/BuildingBase.h"
#include "Engine/AssetManager.h"
#include "Framework/SGameState.h"
#include "Framework/DataAssets/BuildItemDataAsset.h"
#include "GameFramework/GameState.h"
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

	verify((OwningActor = GetOwner()) != nullptr);
	verify((WorldContext = OwningActor->GetWorld()) != nullptr);
	verify((SPlayer = Cast<ASPlayerController>(UGameplayStatics::GetPlayerController(WorldContext, 0))) != nullptr);
}

void UBuildComponent::UpdatePlacementStatus()
{
	if (!ClientBuildObject)
	{
		return;
	}

	TArray<AActor*> OverlappingActors;
	ClientBuildObject->GetOverlappingActors(OverlappingActors);
	bIsPlaceable = OverlappingActors.Num() <= 0;
	ClientBuildObject->UpdateOverlayMaterial(bIsPlaceable);
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

void UBuildComponent::ClientEnterBuildPlacementMode(UBuildItemDataAsset* BuildItemData)
{
	if (!SPlayer || !BuildItemData || !WorldContext)
	{
		return;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SPlayer->GetMousePositionOnTerrain());
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ClientBuildObject = WorldContext->SpawnActor<ABuildable>(BuildItemData->BuildClass.LoadSynchronous(),
	                                                         SpawnTransform, SpawnParams);
	if (ClientBuildObject)
	{
		ClientBuildObject->Init(BuildItemData);
	}

	OnBuildModeEnterEvent.Broadcast();
}

void UBuildComponent::ServerBuildDeploy(UBuildItemDataAsset* BuildData, const FTransform& Location)
{
	if (!BuildData)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	BuildObject = WorldContext->SpawnActor<ABuildable>(BuildData->BuildClass.LoadSynchronous(), Location,
	                                                   SpawnParameters);
	if (BuildObject)
	{
		BuildObject->Init(BuildData, Building);
		BuildObject->OnBuildCompleteEvent.AddDynamic(this, &UBuildComponent::OnBuildComplete);
	}
}

void UBuildComponent::ServerBuildComplete(UBuildItemDataAsset* BuildingData, const FTransform& Location)
{
	if (!OwningActor || !SPlayer || !WorldContext)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (const TSubclassOf<AActor> BuiltClass = BuildingData->BuiltClass.LoadSynchronous())
	{
		if (AActor* BuiltObject = WorldContext->SpawnActor<AActor>(BuiltClass, Location, SpawnParameters))
		{
			if (ASGameState* GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(OwningActor)))
			{
				GameState->AddPlacedObject(FWorldSelectableData(BuiltObject, SPlayer));
			}

			if (ABuildingBase* Building = Cast<ABuildingBase>(BuiltObject))
			{
				Building->SetData(BuildingData);
			}
		}
	}
}

void UBuildComponent::OnBuildComplete(const TEnumAsByte<EBuildState> BuildState)
{
	if (!BuildObject)
	{
		return;
	}
	if (BuildState == BuildComplete)
	{
		ServerBuildComplete(BuildObject->GetBuildItemData(), BuildObject->GetTransform());
	}
	else
	{
		BuildObject->Destroy();
	}
}

void UBuildComponent::EnterBuildPlacementMode(UBuildItemDataAsset* BuildItemData)
{
	if (!SPlayer)
	{
		return;
	}

	SPlayer->SetInputBuildMode();
	ClientEnterBuildPlacementMode(BuildItemData);
}

void UBuildComponent::LoadBuildData()
{
	verify((AssetManager = UAssetManager::GetIfInitialized()) != nullptr);

	const FPrimaryAssetType AssetType("BuildData");
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

void UBuildComponent::ExitBuildMode()
{
	if (!SPlayer)
	{
		return;
	}

	SPlayer->SetInputBuildMode(false);

	if (ClientBuildObject)
	{
		ClientBuildObject->Destroy();
	}
}

void UBuildComponent::BuildDeploy()
{
	if (!ClientBuildObject)
	{
		return;
	}
	if (bIsPlaceable)
	{
		ExitBuildMode();
	}
	else
	{
		//cant build notification
	}
}


// Called every frame
void UBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!ClientBuildObject || !SPlayer)
	{
		return;
	}
	const FVector MouseLocationOnTerrain = SPlayer->GetMousePositionOnTerrain();
	if (ClientBuildObject->GetActorLocation() != MouseLocationOnTerrain)
	{
		ClientBuildObject->SetActorLocation(MouseLocationOnTerrain);
	}

	UpdatePlacementStatus();
}
