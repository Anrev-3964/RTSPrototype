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

EFaction ARTSPlayerState::GetFaction() const
{
	return CurrentFaction;
}

void ARTSPlayerState::SetCurrentFaction(EFaction NewFaction)
{
	if (CurrentFaction != NewFaction)
	{
		CurrentFaction = NewFaction;
	}
}


void ARTSPlayerState::BeginPlay()
{
	Super::BeginPlay();
}
