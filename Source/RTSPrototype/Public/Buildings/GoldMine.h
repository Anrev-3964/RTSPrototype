// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildable.h"
#include "ITriggerBoxArea.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Core/FactionsUtils.h"
#include "Core/GoldMineData.h"
#include "Core/Selectable.h"
#include "Framework/RTSPlayerState.h"
#include "Framework/DataAssets/BuildData.h"
#include "GoldMine.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API AGoldMine : public AActor,public IFactionsUtils,public ISelectable,public IITriggerBoxArea
{
	GENERATED_BODY()
public:
	AGoldMine(const FObjectInitializer& ObjectInitializer);
	void BeginPlay();
	void SetMineFromDataAsset();

private:
	UPROPERTY()  
	EFaction CurrentFaction = {EFaction::Neutral};

	UPROPERTY()  
	EFaction NewOwnerFaction = {EFaction::Neutral};

	UPROPERTY(EditAnywhere, Category = "Data")
	UGoldMineData* GoldMineData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Mine Settings",meta = (Tooltip = "La quantita di oro estraibile dalla miniera (default : 1000)"))
	int16  GoldAmount;
	UPROPERTY()
	int CurrentGoldAmount;

	UPROPERTY()
	int GoldEstractAmount = {5};
	UPROPERTY()
	ARTSPlayerState* OwnerPlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	TSoftObjectPtr<UStaticMesh> CompletedMineMesh;

	UPROPERTY()
	TSubclassOf<AActor> BuildingActorCompleteClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* RootComponentIntermediate;

public:
	UFUNCTION()
	int GetGoldAmount() const ;
	UFUNCTION()
	int GetCurrentGoldAmount() const;
	
	/**IFactionUtils Interface**/
	virtual EFaction GetFaction()const override;
	virtual void SetCurrentFaction(EFaction NewFaction) override;

	
	ARTSPlayerState* GetOwnerPlayerState() const;
	/**End IFactionUtils Interface**/

	UFUNCTION(BlueprintCallable)
	void PreBuildMine(AActor*  OverlapActor);

	UFUNCTION()
	void MineStarted(ABuildable* Buildable);

	UFUNCTION()
	void MineCompleted(const TEnumAsByte<EBuildState> BuildState);

	UFUNCTION()
	void EstractGold();

	UFUNCTION(BlueprintCallable)
	int GetGoldEstractionAmount();
	void SetStaticMeshFromActor();

	/** ISelecatbleInterface UNUSED FOR NOW**/
	UFUNCTION()
	virtual void Select() override;
	UFUNCTION()
	virtual void DeSelect() override;
	UFUNCTION()
	virtual void Highlight(const bool Highlight) override;
	/** ITriggerBoxArea **/
	virtual bool ActorIsATriggerArea() override;
};

