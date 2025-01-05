// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/GoldMineConstructor.h"

#include "Framework/DataAssets/BuildItemDataAsset.h"

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
