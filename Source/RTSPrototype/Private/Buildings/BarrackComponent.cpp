// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/BarrackComponent.h"

#include "Core/SPlayerController.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "RTSPrototype/RTSPrototypeCharacter.h"

// Sets default values for this component's properties
UBarrackComponent::UBarrackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBarrackComponent::BeginPlay()
{
	Super::BeginPlay();

	verify((OwningActor = GetOwner()) != nullptr);
	verify((WorldContext = OwningActor->GetWorld()) != nullptr);
	verify((SPlayer = Cast<ASPlayerController>(UGameplayStatics::GetPlayerController(WorldContext, 0))) != nullptr);
	verify((PlayerState = Cast<ARTSPlayerState>(UGameplayStatics::GetPlayerState(WorldContext, 0))) != nullptr);
}

void UBarrackComponent::SpawnUnit(UUnitData* UnitData)
{
	if (!SPlayer || !UnitData || !WorldContext)
	{
		return;
	}

	if (UnitData->GetGoldCost() > PlayerState->GetGold())
	{
		UE_LOG(LogTemp, Error, TEXT("NOT ENOUGH GOLD"));
		return;
	}

	PlayerState->RemoveGold(UnitData->GetGoldCost());

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(OwningActor->GetActorLocation() + FVector(200, 0, 0));
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (UnitData && UnitData->UnitClass)
	{
		ARTSPrototypeCharacter* SpawnedUnit = WorldContext->SpawnActor<ARTSPrototypeCharacter>(
			UnitData->UnitClass.LoadSynchronous(), SpawnTransform, SpawnParams);
		if (SpawnedUnit)
		{
			UE_LOG(LogTemp, Log, TEXT("Unit Spawned Successfully!"));
		}
	}
	
}

void UBarrackComponent::LoadUnitData()
{
	UE_LOG(LogTemp, Log, TEXT("Loading unit data"));
	verify((AssetManager = UAssetManager::GetIfInitialized()) != nullptr);

	const FPrimaryAssetType AssetType("UnitData");
	TArray<FPrimaryAssetId> UnitDataAssets;

	AssetManager->GetPrimaryAssetIdList(AssetType, UnitDataAssets);

	UE_LOG(LogTemp, Log, TEXT("Found %d UnitData assets."), UnitDataAssets.Num());

	for (const FPrimaryAssetId& AssetId : UnitDataAssets)
	{
		const UObject* AssetObject = AssetManager->GetPrimaryAssetObject(AssetId);
		if (const UUnitData* UnitData = Cast<UUnitData>(AssetObject))
		{
			//UnitItemsData.Add(UnitData);
			UE_LOG(LogTemp, Log, TEXT("Added unit asset: %s"), *AssetId.ToString());
		}
	}
}

