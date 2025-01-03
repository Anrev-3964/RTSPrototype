// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/UnitData.h"
#include "Framework/RTSPlayerState.h"
#include "BarrackComponent.generated.h"

class ASPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTSPROTOTYPE_API UBarrackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBarrackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	ARTSPlayerState* PlayerState;
	UPROPERTY()
	ASPlayerController* SPlayer;
	UPROPERTY()
	AActor* OwningActor;
	UPROPERTY()
	UWorld* WorldContext;
	/*UPROPERTY()
	UUnitData* UnitData;*/
	UPROPERTY()
	UAssetManager* AssetManager;
	UPROPERTY()
	TArray<UUnitData*> UnitItemsData;

	void SpawnUnit(UUnitData* UnitDataAsset);
	void LoadUnitData();
	
};
