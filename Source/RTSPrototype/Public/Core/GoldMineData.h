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

	/** Tier 1 settings **/ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 1 Settings")
	int TierOneGoldEstractionAmount = {1};

	/** Tier 2 settings **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 2 Settings")
	int TierTwoGoldEstractionAmount = {2};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 2 Settings")
	float TierTwoBuildingUpgradeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 2 Settings")
	float TierTwoBuildingUpgradeCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 2 Settings")
	UNiagaraSystem* TierTwoCompleteNiagaraSystem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "Tier 2 Settings")
	USoundBase* TierTwoCompletedAudioClip;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "General Settings")
	USoundBase* GoldCollectedAudioClip;
	/** Tier 3 settings **/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 3 Settings")
	int TierThreeGoldEstractionAmount = {3};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 3 Settings")
	float TierThreeBuildingUpgradeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 3 Settings")
	float TierThreeBuildingUpgradeCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tier 3 Settings")
	UNiagaraSystem* TierThreeCompleteNiagaraSystem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "Tier 3 Settings")
	USoundBase* TierThreeCompletedAudioClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrading")
	UNiagaraSystem* UpgradingNiagaraSystem;
	
};
