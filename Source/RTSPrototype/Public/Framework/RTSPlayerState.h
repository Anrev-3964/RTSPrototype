// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RTSPlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoldChanged);

UCLASS()
class RTSPROTOTYPE_API ARTSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void AddGold(int GoldGained);
	void RemoveGold(int GoldSpent);
	int GetGold() { return Gold; };

	void SetGold(int NewGold)
	{
		Gold = NewGold;
		UE_LOG(LogTemp, Warning, TEXT(" New Gold: %d"), Gold);
		OnGoldChanged.Broadcast();
	}

	FOnGoldChanged OnGoldChanged;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int Gold = 300;
	virtual void BeginPlay() override;
};
