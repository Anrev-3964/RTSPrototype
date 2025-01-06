// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GoldMineData.h"

float UGoldMineData::GetMineGoldAmount() const
{
	if (MineGoldAmount <= 0 )
	{
		return 10.f;
	}
	return  MineGoldAmount;
}
