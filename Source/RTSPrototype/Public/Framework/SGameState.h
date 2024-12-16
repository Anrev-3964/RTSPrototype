// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/BuildData.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()
	public:
	ASGameState();
	virtual void Tick(float DeltaTime) override;
	void AddPlacedObject(const FWorldSelectableData NewObject);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	TArray<FWorldSelectableData> PlacedObjects;
};
