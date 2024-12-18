// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	TEnumAsByte<EBuildAssetFilter> BuildAssetFilter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	FText TitleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	TSoftObjectPtr<UTexture2D> Image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
	TSoftClassPtr<AActor> BuildClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
	TSoftClassPtr<AActor> BuiltClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	TArray<TSoftObjectPtr<UStaticMesh>> BuildMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	TSoftObjectPtr<UStaticMesh> BuildingMeshComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	TSoftObjectPtr<UMaterialInstance> PlaceMaterial;
};
