// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildData.Generated.h"

UENUM()
enum EBuildAssetFilter
{
	Filter1,
	Filter2
};

UENUM()
enum EBuildState
{
	NoBuild,
	Building,
	BuildAborted,
	BuildComplete
};

USTRUCT()
struct FWorldSelectableData
{
	GENERATED_BODY()

public:
	FWorldSelectableData(): Object(nullptr), OwningPlayer(nullptr)
	{
	}
	FWorldSelectableData(AActor* InObject, APlayerController* SPlayer): Object(InObject), OwningPlayer(SPlayer) {}

	UPROPERTY()
	AActor* Object;
	UPROPERTY()
	APlayerController* OwningPlayer;
};
