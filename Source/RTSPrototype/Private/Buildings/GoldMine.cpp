// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/GoldMine.h"


AGoldMine::AGoldMine(const FObjectInitializer& ObjectInitializer)
{
	CurrentFaction = EFaction::Team1;
	GoldAmount = 9999;
	CurrentGoldAmount = GoldAmount;
}

AGoldMine::AGoldMine(EFaction Faction, int16 GoldAmount)
{
	CurrentFaction = Faction;
	CurrentGoldAmount = GoldAmount;
}

int AGoldMine::GetGoldAmount() const
{
	if (GoldAmount <= 0)
	{
		return 1000;
	}
	return GoldAmount;
}

int AGoldMine::GetCurrentGoldAmount() const 
{
	return CurrentGoldAmount;
}
