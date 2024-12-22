// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UnitData.generated.h"


UCLASS(BlueprintType)
class RTSPROTOTYPE_API UUnitData : public UDataAsset
{
	GENERATED_BODY()
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

	//da aggiungere : Behaivor tree, anim motage,velocita di movimento
public:
	/**Get Fuctions**/
	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	FString GetName() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetAttack() const;
	/**Get Fuctions**/
};
