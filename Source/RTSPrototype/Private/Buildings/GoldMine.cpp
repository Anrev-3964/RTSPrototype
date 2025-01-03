// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/GoldMine.h"

#include "GameFramework/GameStateBase.h"


AGoldMine::AGoldMine(const FObjectInitializer& ObjectInitializer)
{
	CurrentFaction = EFaction::Team1;
	GoldAmount = 9999;
	CurrentGoldAmount = GoldAmount;
}

AGoldMine::AGoldMine(EFaction Faction, int GoldAmount)
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

//NOTA : pe rora la quantita di oro estraibile non diminuisce a dogni estrazione
void AGoldMine::EstractGold(int GoldAmountToEstract)
{
	if (!OwnerPlayerState) return;
	if (GoldAmount >= 0)
	{
		OwnerPlayerState->AddGold(GoldAmountToEstract);
	}
}
