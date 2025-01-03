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

	UPROPERTY(EditDefaultsOnly, Category = "Unit Stats",meta = (Tooltip = "Il costo in valuta per creare l'unita(UnitCost = 1 se UnitCost < 1) "))
	int UnitCost;
	
	UPROPERTY(EditDefaultsOnly, Category = "Unit Stats",meta = (Tooltip = "La quantita di oro che l'unita puo estrare per volta (se puo farlo. GoldEstractionCapacity = 10 se GoldEstractionCapacity <= 10)"))
	int GoldEstractionCapacity; 

	UPROPERTY(EditDefaultsOnly, Category = "Unit Settings",meta = (Tooltip = "Il tempo di addestramenot di un unita (TrainingTime = 1 se : TrainingTime < 1)"))
	float TrainingTime;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Settings",meta = (Tooltip = "True : l'unita puo interagire con la miniera e raccogliere oro"))
	bool CanMineGold;
	
	UPROPERTY(EditDefaultsOnly, Category = "Unit Settings",meta = (Tooltip = "l'animazione usata dal unita quando attaca"))
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Settings",meta = (Tooltip = "l'animazione usata dal unita quando Raccoglie oro"))
	UAnimMontage* MiningMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Unit AI ",meta = (Tooltip = "Range di attaco per l'unita (AttackRange = 200 se : AttackRange < 200)"))
	float AttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "Unit AI ",meta = (Tooltip = "Range tolleranza per il navmesh dell'unita (UnitAcceptanceRadius = 100 se : UnitAcceptanceRadius < 100)"))
	float UnitAcceptableRadius;

	
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
	int GetUnitCost() const;
	
	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	int GetGoldEstractionCapacity() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	bool GetCanMineGold() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetTrainingTime() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	UAnimMontage* GetAttackMontage()  const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	UAnimMontage* GetMiningMontage()  const;
	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetAttackDelay()  const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetAttackRange()  const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetUnitAcceptableRadius()  const;
	
};
