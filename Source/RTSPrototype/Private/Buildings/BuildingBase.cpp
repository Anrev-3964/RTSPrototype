// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/BuildingBase.h"

#include "Chaos/Deformable/MuscleActivationConstraints.h"

// Sets default values
ABuildingBase::ABuildingBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
}

void ABuildingBase::SetData(UBuildItemDataAsset* BuildingData)
{
	if (BuildingData)
	{
		Data = BuildingData;
	}

	if (!StaticMesh || BuildingData)
	{
		return;
	}
	
	if (UStaticMesh* CompletedMesh = Data->BuildingMeshComplete.LoadSynchronous())
	{
		StaticMesh->SetStaticMesh(CompletedMesh);
	} 
}

// Called when the game starts or when spawned
void ABuildingBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

