// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UnitData.h"

#include "CrossCompilerCommon.h"

FString UUnitData::GetName() const
{
	return Name;
}

UTexture2D* UUnitData::GetImage() const
{
	return UnitImage;
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

int UUnitData::GetGoldCost() const
{
	if (GoldCost <= 0)
	{
		return 100;
	}
	return GoldCost;
}

float UUnitData::GetSpawnDelay() const
{
	if (SpawnDelay <= 0)
	{
		return 1;
	}
	return SpawnDelay;
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

int UUnitData::GetGoldEstractionCapacity() const
{
	if (GoldEstractionCapacity <= 0)
	{
		return 10;
	}
	return GoldEstractionCapacity;
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
	if (AttackMontage)
	{
		return AttackMontage;	
	}
	return nullptr;
}

UAnimMontage* UUnitData::GetMiningMontage() const
{
	if (MiningMontage)
	{
		return MiningMontage;
	}
	return nullptr;
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
	return UnitAcceptableRadius;
}

