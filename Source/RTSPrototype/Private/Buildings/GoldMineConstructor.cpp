// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/GoldMineConstructor.h"

#include "Framework/DataAssets/BuildItemDataAsset.h"
#include "UObject/FastReferenceCollector.h"

UStaticMesh* AGoldMineConstructor::GetFinalMesh()
{
	if (BuildData)
	{
		// Carica sincronicamente la mesh a partire dal TSoftObjectPtr
		UStaticMesh* FinalMesh = BuildData->BuildingMeshComplete.LoadSynchronous();

		// Restituisci la mesh
		return FinalMesh;
	}
	return nullptr;
}

void AGoldMineConstructor::BeginPlay()
{
	Super::BeginPlay();
	SetCurrentFaction(EFaction::Team1);
	OnBuildCompleteEvent.AddDynamic(this, &AGoldMineConstructor::DestroySelf);
}

void AGoldMineConstructor::DestroySelf(const TEnumAsByte<EBuildState> NewBuildState)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red , "Buildable Distrutto");
	}
	if (WeakActorPtr.IsValid())
	{
		AActor* ActorToDestroy = WeakActorPtr.Get(); // Ottieni il puntatore all'attore

		if (ActorToDestroy) // Verifica se l'attore è valido
		{
			ActorToDestroy->Destroy(); // Distruggi l'attore
		}
	}
	Destroy();
}




