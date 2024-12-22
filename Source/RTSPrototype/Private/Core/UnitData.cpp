// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UnitData.h"

FString UUnitData::GetName() const
{
	return Name;
}

float UUnitData::GetMaxHealth() const
{
	return MaxHealth;
}

float UUnitData::GetAttack() const
{
	return Attack;
}
