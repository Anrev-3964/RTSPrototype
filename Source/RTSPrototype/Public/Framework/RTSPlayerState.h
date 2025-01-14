// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/FactionsUtils.h"
#include "GameFramework/PlayerState.h"
#include "RTSPlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoldChanged);

UCLASS()
class RTSPROTOTYPE_API ARTSPlayerState : public APlayerState,public IFactionsUtils
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddGold(int GoldGained);
	UFUNCTION(BlueprintCallable)
	void RemoveGold(int GoldSpent);
	int GetGold() { return Gold; };

	void SetGold(int NewGold)
	{
		Gold = NewGold;
		UE_LOG(LogTemp, Warning, TEXT(" New Gold: %d"), Gold);
		OnGoldChanged.Broadcast();
	}

	FOnGoldChanged OnGoldChanged;

	/**IFactionUtils Interface**/
	virtual EFaction GetFaction()const override;
	/**End IFactionUtils Interface**/
	void SetCurrentFaction(EFaction NewFaction); //TO DO : add this funciton in the interface

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int Gold = 500;
	virtual void BeginPlay() override;

	UPROPERTY()  
	EFaction CurrentFaction = {EFaction::Team1};
};
