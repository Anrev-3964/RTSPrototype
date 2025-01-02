// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/FactionsUtils.h"
#include "Core/Selectable.h"
#include "Framework/DataAssets/BuildItemDataAsset.h"
#include "GameFramework/Actor.h"
#include "BuildingBase.generated.h"

UCLASS()
class RTSPROTOTYPE_API ABuildingBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildingBase();
	void SetData(UBuildItemDataAsset* BuildingData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UBuildItemDataAsset* Data;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
