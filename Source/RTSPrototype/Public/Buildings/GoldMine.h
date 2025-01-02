// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildable.h"
#include "Buildings/BuildingBase.h"
#include "Core/FactionsUtils.h"
#include "GoldMine.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API AGoldMine : public ABuildable
{
	GENERATED_BODY()
public:
	AGoldMine(const FObjectInitializer& ObjectInitializer);
	AGoldMine(EFaction Faction = EFaction::Team1,int16 GoldAmount = 9999);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Mine Settings",meta = (Tooltip = "La quantita di oro estraibile dalla miniera (default : 1000)"))
	int16  GoldAmount;
	UPROPERTY()
	int CurrentGoldAmount;

public:
	UFUNCTION()
	int GetGoldAmount() const ;
	UFUNCTION()
	int GetCurrentGoldAmount() const;
};
