// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SGameState.h"

ASGameState::ASGameState()
{
	PrimaryActorTick.bCanEverTick = true;
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
