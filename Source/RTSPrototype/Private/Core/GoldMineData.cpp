// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GoldMineData.h"

UStaticMesh* UGoldMineData::GetMineStaticMesh() const
{
	if (!MineStaticMesh)
	{
		return nullptr;
		//restituire una mesh di default
	}
	return MineStaticMesh;
}

float UGoldMineData::GetMineGoldAmount() const
{
	if (MineGoldAmount <= 0 )
	{
		return 1000.f;
	}
	return  MineGoldAmount;
}
