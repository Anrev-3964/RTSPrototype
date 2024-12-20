// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/PlacementPreview.h"

// Sets default values
APlacementPreview::APlacementPreview()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void APlacementPreview::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("Building spawned: %s"), *GetName());
}

// Called every frame
void APlacementPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
