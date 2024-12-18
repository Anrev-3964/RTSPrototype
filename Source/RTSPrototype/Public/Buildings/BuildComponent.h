// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/SPlayerController.h"
#include "BuildComponent.generated.h"


class ABuildable;
class UBuildItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildModeEnterDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RTSPROTOTYPE_API UBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBuildComponent();

protected:
	//TO CONTINUE AFTER TUTORIAL 2
	// Called when the game starts
	virtual void BeginPlay() override;
	void UpdatePlacementStatus();

	UFUNCTION()
	void OnBuildDataLoaded(TArray<FPrimaryAssetId> BuildAssetsIds);

	UPROPERTY()
	UAssetManager* AssetManager;
	UPROPERTY()
	TArray<FPrimaryAssetId> BuildItemsData;
	UPROPERTY()
	ASPlayerController* SPlayer;
	UPROPERTY()
	AActor* OwningActor;
	UPROPERTY()
	UWorld* WorldContext;
	UPROPERTY()
	ABuildable* BuildObject;
	UPROPERTY()
	ABuildable* ClientBuildObject; //probably server, to remove
	UPROPERTY()
	bool bIsPlaceable = false;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	static UBuildComponent* FindBuildComponent(const AActor* Actor)
	{
		return (Actor ? Actor->FindComponentByClass<UBuildComponent>() : nullptr);
	}

	void EnterBuildPlacementMode(UBuildItemDataAsset* BuildItemData);
	void LoadBuildData();
	TArray<FPrimaryAssetId> GetBuildData() const { return BuildItemsData; }
	FOnBuildModeEnterDelegate OnBuildModeEnterEvent;
};
