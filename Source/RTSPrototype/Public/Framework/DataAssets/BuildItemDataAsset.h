// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Engine/DataAsset.h"
#include "BuildData.h"
#include "BuildItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API UBuildItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data Settings")
	FPrimaryAssetType DataType;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(DataType, GetFName());
	}

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	TEnumAsByte<EBuildAssetFilter> BuildAssetFilter;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	FText TitleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	TSoftObjectPtr<UTexture2D> Image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
	TSoftClassPtr<AActor> BuildClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
	TSoftClassPtr<AActor> BuiltClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int GoldCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool CanBeBuiltCloseToGoldSources;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	TArray<TSoftObjectPtr<UStaticMesh>> BuildMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	TSoftObjectPtr<UStaticMesh> BuildingMeshComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	TSubclassOf<AActor> BuildingActorComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building In Progress Effect Configuration")
	UNiagaraSystem* BuildingInProgressNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building In Progress Effect Configuration")
	float BuildingInProgressNiagara_LoopDuration = {1.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building In Progress Effect Configuration")
	FVector BuildingInProgressNiagara_SpawnOffset = {FVector::Zero()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Completed Configuration")
	UNiagaraSystem* BuildingCompletedNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Completed Configuration")
	float BuildingCompletedNiagara_LoopDuration = {1.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Completed Configuration")
	FVector BuildingCompletedNiagara_SpawnOffset = {FVector::Zero()};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	TSoftObjectPtr<UMaterialInstance> PlaceMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Selection")
	TSoftObjectPtr<UMaterialInstance> HighlightMaterial;
};
