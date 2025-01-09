// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	TSoftObjectPtr<UTexture2D> Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	FText TitleText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TSoftObjectPtr<UStaticMesh> MeshReference;

	/**
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier Configuration")
	TSubclassOf<AActor> TierOneBuildingActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier Configuration")
	TSubclassOf<AActor> TierTwoBuildingActorClass;
	**/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 2 Settings")
	float TierTwoBuildingUpgradeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 2 Settings")
	float TierTwoBuildingUpgradeCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 2 Settings")
	UNiagaraSystem* TierTwoCompleteNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 3 Settings")
	float TierThreeBuildingUpgradeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 3 Settings")
	float TierThreeBuildingUpgradeCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 3 Settings")
	UNiagaraSystem* TierThreeCompleteNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrading")
	UNiagaraSystem* UpgradingNiagaraSystem;

	
	
};
