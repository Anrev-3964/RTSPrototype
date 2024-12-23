// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RTSPlayerState.h"

void ARTSPlayerState::AddGold(int GoldGained)
{
	Gold += GoldGained;
	SetGold(Gold);
}

void ARTSPlayerState::RemoveGold(int GoldSpent)
{
	Gold -= GoldSpent;
	SetGold(Gold);
}

void ARTSPlayerState::BeginPlay()
{
	Super::BeginPlay();
}
