// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UnitData.h"

FString UUnitData::GetName() const
{
	return Name;
}

float UUnitData::GetMaxHealth() const
{
	if (MaxHealth <= 0)
	{
		return 10;
	}
	return MaxHealth;
}

float UUnitData::GetAttack() const
{
	if (Attack <= 0)
	{
		return 1;
	}
	return Attack;
}

float UUnitData::GetMaxMovementSpeed() const
{
	if (MaxMovementSpeed <= 0)
	{
		return 600;
	}
	return MaxMovementSpeed;
}

int UUnitData::GetUnitCost() const
{
	if (UnitCost <= 1)
	{
		return 1;
	}
	return UnitCost;
}

int UUnitData::GetGoldTransportCapacity() const
{
	if (GoldTransportCapacity <= 10)
	{
		return 10;
	}
	return GoldTransportCapacity;
}

bool UUnitData::GetCanMineGold() const
{
	return CanMineGold;
}

float UUnitData::GetTrainingTime() const
{
	if (TrainingTime  <= 1)
	{
		return  1;
	}
	return TrainingTime;
}

UAnimMontage* UUnitData::GetAttackMontage() const
{
	return AttackMontage;
}

float UUnitData::GetAttackDelay() const
{
	return AttackDelay;
}

float UUnitData::GetAttackRange() const
{
	if (AttackRange < 200)
	{
		return 200.f;
	}
	return AttackRange;
}

float UUnitData::GetUnitAcceptableRadius() const
{
	if (UnitAcceptableRadius <= 100)
	{
		return 100;
	}
	return  UnitAcceptableRadius;
}

