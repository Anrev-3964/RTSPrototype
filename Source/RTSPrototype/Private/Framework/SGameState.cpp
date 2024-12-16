// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SGameState.h"

#include "Net/UnrealNetwork.h"

ASGameState::ASGameState()
{
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	/*Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASGameState, PlacedObjects);*/ //SERVER
}
void ASGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGameState::AddPlacedObject(const FWorldSelectableData NewObject)
{
	PlacedObjects.Add(NewObject);
}

void ASGameState::BeginPlay()
{
	Super::BeginPlay();
}
