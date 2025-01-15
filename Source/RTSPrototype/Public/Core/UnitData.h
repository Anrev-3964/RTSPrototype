// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "Animation/AnimMontage.h"
#include "UnitData.generated.h"

UCLASS(BlueprintType)
class RTSPROTOTYPE_API UUnitData : public UDataAsset
{
	GENERATED_BODY()
	//To DO : informazioni da aggiungere : fazione, animation Attack montage
	// Proprietà private del Data Asset
	UPROPERTY(EditDefaultsOnly, Category = "Unit Infomration")
	FString Name;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Information")
	FString Description;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Information")
	UTexture2D* UnitImage;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Stats")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Stats")
	float Attack;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Stats", meta = (Tooltip = "Delay in secondi tra un attacco e l'altro in consecutiva"))
	float AttackDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Stats")
    int GoldCost;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Stats",meta = (Tooltip = "la velocita massima che l'unita puo raggiungere"))
	float MaxMovementSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Stats", meta =(ToolTip = "Spawn Delay"))
    float SpawnDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Settings",meta = (Tooltip = "l'animazione usata dal unita quando attaca"))
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Settings",meta = (Tooltip = "l'animazione usata dal unita quando Raccoglie oro"))
    UAnimMontage* MiningMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Unit Settings",meta = (Tooltip = "True : l'unita puo interagire con la miniera e raccogliere oro"))
    bool CanMineGold;
	UPROPERTY(EditDefaultsOnly, Category = "Unit AI ",meta = (Tooltip = "Range di attaco per l'unita (AttackRange = 200 se : AttackRange < 200)"))
	float AttackRange;
	UPROPERTY(EditDefaultsOnly, Category = "Unit AI ",meta = (Tooltip = "Range tolleranza per il navmesh dell'unita (UnitAcceptanceRadius = 100 se : UnitAcceptanceRadius < 100)"))
	float UnitAcceptableRadius;

	UPROPERTY(EditDefaultsOnly,Category = "Audio")
	float RandomMaxPitch;
	
	UPROPERTY(EditDefaultsOnly,Category = "Audio")
	float RandomMinPitch;

public:
	/**Get Fuctions**/
	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	FString GetName() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	UTexture2D* GetImage() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	int GetGoldCost() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetSpawnDelay() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetMaxMovementSpeed() const;
	
	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
    bool GetCanMineGold() const;
    
	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	UAnimMontage* GetAttackMontage() const;

	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	UAnimMontage* GetMiningMontage()  const;
	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetAttackDelay()  const;
	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetAttackRange()  const;
	UFUNCTION(BlueprintCallable, Category = "Get Unit Data")
	float GetUnitAcceptableRadius()  const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "Audio")
	TArray<USoundBase*> SelectedUnitAudioClips;

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "Audio")
	USoundBase* UnitAttackAudioClip;

	/**
	UFUNCTION(Category= "Audio")
	float GetRandomMaxPitch() const;

	UFUNCTION(Category= "Audio")
	float  GetRandomMinPitch() const;
	**/
	
};
