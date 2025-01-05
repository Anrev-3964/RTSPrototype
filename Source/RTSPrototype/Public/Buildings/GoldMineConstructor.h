// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildings/Buildable.h"
#include "GoldMineConstructor.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API AGoldMineConstructor : public ABuildable
{
	GENERATED_BODY()

public:
	UFUNCTION()
	UStaticMesh* GetFinalMesh();
};
