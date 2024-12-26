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

UAnimMontage* UUnitData::GetAttackMontage() const
{
	return AttackMontage;
}

/**
float UUnitData::GetSightRadius() const
{
	return SightRadius;
}

float UUnitData::GetLoseSightRadius() const
{
	return LoseSightRadius;
}
**/
