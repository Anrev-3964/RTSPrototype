// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GoldMineData.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API UGoldMineData : public UDataAsset
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, Category = "Mine Settings",meta = (Tooltip = "quantita di oro estraibile dalla miniera "))
	float MineGoldAmount;

public:
	UFUNCTION(BlueprintCallable, Category = "Mine Settings")
	float GetMineGoldAmount() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TSoftObjectPtr<UStaticMesh> MeshReference;
};