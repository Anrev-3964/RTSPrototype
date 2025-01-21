// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/BuildComponent.h"

#include "Buildings/Buildable.h"
#include "Buildings/BuildingBase.h"
#include "Buildings/ITriggerBoxArea.h"
#include "Engine/AssetManager.h"
#include "Framework/RTSPlayerState.h"
#include "Framework/SGameState.h"
#include "Framework/DataAssets/BuildItemDataAsset.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Slate/SGameLayerManager.h"

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
	verify((PlayerState = Cast<ARTSPlayerState>(UGameplayStatics::GetPlayerState(WorldContext, 0))) != nullptr);
}

void UBuildComponent::UpdatePlacementStatus()
{
	if (!ClientBuildObject)
	{
		return;
	}

	if (UBuildItemDataAsset* BuildItemData = ClientBuildObject->GetBuildItemData())
	{
		TArray<AActor*> OverlappingActors;
		ClientBuildObject->GetOverlappingActors(OverlappingActors);

		if (BuildItemData->CanBeBuiltCloseToGoldSources)
		{
			bIsPlaceable = false;
			
			for (AActor* Actor : OverlappingActors)
			{
				if (Actor != ClientBuildObject)
				{
					if (IITriggerBoxArea* TriggerBox = Cast<IITriggerBoxArea>(Actor))
					{
						bIsPlaceable = TriggerBox->ActorIsATriggerArea(); 
						ClientBuildObject->UpdateOverlayMaterial(bIsPlaceable);
						break;
					}
				}
					
			}
		}
		else
		{
			bIsPlaceable = OverlappingActors.Num() <= 0;
			ClientBuildObject->UpdateOverlayMaterial(bIsPlaceable);
		}
	}
}

void UBuildComponent::OnBuildDataLoaded(TArray<FPrimaryAssetId> BuildAssetsIds)
{
	if (!SPlayer)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Build data loaded with %d assets"), BuildAssetsIds.Num());
	for (const FPrimaryAssetId& Asset : BuildAssetsIds)
	{
		UE_LOG(LogTemp, Log, TEXT("Loaded asset: %s"), *Asset.ToString());
	}

	for (int i = 0; i < BuildAssetsIds.Num(); i++)
	{
		const UObject* AssetObject = AssetManager->GetPrimaryAssetObject(BuildAssetsIds[i]);
		if (!AssetObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get asset object for ID: %s"), *BuildAssetsIds[i].ToString());
			continue;
		}

		const UBuildItemDataAsset* BuildDataAsset = Cast<UBuildItemDataAsset>(AssetObject);
		if (!BuildDataAsset)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to cast asset to UBuildItemDataAsset: %s"),
			       *BuildAssetsIds[i].ToString());
			continue;
		}
		
		BuildItemsData.Add(BuildAssetsIds[i]);
		UE_LOG(LogTemp, Log, TEXT("Added asset: %s"), *BuildAssetsIds[i].ToString());
	}
}

void UBuildComponent::ClientEnterBuildPlacementMode(UBuildItemDataAsset* BuildItemData)
{
	if (!SPlayer || !BuildItemData || !WorldContext)
	{
		return;
	}

	if (BuildItemData->GoldCost > PlayerState->GetGold())
	{
		UE_LOG(LogTemp, Error, TEXT("NOT ENOUGH GOLD"));
		return;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SPlayer->GetMousePositionOnTerrain().Location);
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

	PlayerState->RemoveGold(BuildData->GoldCost);

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
	UE_LOG(LogTemp, Log, TEXT("Loading build data"));
	verify((AssetManager = UAssetManager::GetIfInitialized()) != nullptr);

	const FPrimaryAssetType AssetType("BuildData");
	TArray<FPrimaryAssetId> BuildDataAssets;

	AssetManager->GetPrimaryAssetIdList(AssetType, BuildDataAssets);

	UE_LOG(LogTemp, Log, TEXT("Found %d BuildData assets."), BuildDataAssets.Num());

	if (BuildDataAssets.Num() > 0)
	{
		const TArray<FName> Bundles;
		const FStreamableDelegate FormationDataLoadedDelegate =
			FStreamableDelegate::CreateUObject(this, &UBuildComponent::OnBuildDataLoaded, BuildDataAssets);

		AssetManager->LoadPrimaryAssets(BuildDataAssets, Bundles, FormationDataLoadedDelegate);
		UE_LOG(LogTemp, Log, TEXT("Loading %d BuildData assets."), BuildDataAssets.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No BuildData assets found."));
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

void UBuildComponent::BuildingDeploy()
{
	if (!ClientBuildObject)
	{
		return;
	}
	if (bIsPlaceable)
	{
		FTransform SpawnTransform(ClientBuildObject->GetActorLocation());
		ServerBuildDeploy(ClientBuildObject->GetBuildItemData(), SpawnTransform);
		ExitBuildMode();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot place the building. Overlap detected or invalid position."));
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
	const FVector MouseLocationOnTerrain = SPlayer->GetMousePositionOnTerrain().Location;
	if (ClientBuildObject->GetActorLocation() != MouseLocationOnTerrain)
	{
		ClientBuildObject->SetActorLocation(MouseLocationOnTerrain);
	}

	UpdatePlacementStatus();
}
