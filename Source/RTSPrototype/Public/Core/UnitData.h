// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UnitData.generated.h"


UCLASS(BlueprintType)
class RTSPROTOTYPE_API UUnitData : public UDataAsset
{
	GENERATED_BODY()
	//To DO : informazioni da aggiungere : fazione, animation Attack montage
private:
	// Proprietà private del Data Asset
	UPROPERTY(EditDefaultsOnly, Category = "Unit Infomration")
	FString Name;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Information")
	FString Description;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Stats")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Stats")
	float Attack;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Stats", meta = (Tooltip = "Delay in secondi tra un attacco e l'altro in consecutiva"))
	float AttackDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Stats",meta = (Tooltip = "la velocita massima che l'unita puo raggiungere"))
	float MaxMovementSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Settings",meta = (Tooltip = "l'animazione usata dal unita quando attaca"))
	UAnimMontage* AttackMontage;

	/**
	UPROPERTY(EditDefaultsOnly, Category = "Unit Percetion")
	float SightRadius;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Percetion")
	float LoseSightRadius;
	**/
	//da aggiungere : Behaivor tree, anim motage,velocita di movimento
public:
	/**Get Fuctions**/
	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	FString GetName() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetMaxMovementSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	UAnimMontage* GetAttackMontage()  const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetAttackDelay()  const;
	/**
	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetSightRadius() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetLoseSightRadius() const;
	**/
	/**Get Fuctions**/
};
