// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ITriggerBoxArea.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Core/FactionsUtils.h"
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

private:
	UPROPERTY()  
	EFaction CurrentFaction = {EFaction::Team2};

	UPROPERTY(EditDefaultsOnly, Category = "Mine Settings",meta = (Tooltip = "La quantita di oro estraibile dalla miniera (default : 1000)"))
	int16  GoldAmount;
	UPROPERTY()
	int CurrentGoldAmount;

	UPROPERTY()
	ARTSPlayerState* OwnerPlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMesh* DefaultMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMesh* NewMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* RootComponentIntermediate;

public:
	UFUNCTION()
	int GetGoldAmount() const ;
	UFUNCTION()
	int GetCurrentGoldAmount() const;
	
	/**IFactionUtils Interface**/
	virtual EFaction GetFaction()const override;
	ARTSPlayerState* GetOwnerPlayerState() const;
	/**End IFactionUtils Interface**/

	void SetCurrentFaction(EFaction NewFaction); //TO DO : add this funciton in the interface

	UFUNCTION(BlueprintCallable)
	void BuildMine(AActor*  OverlapActor);

	UFUNCTION()
	void MineCompleted(TEnumAsByte<EBuildState> BuildState);

	UFUNCTION()
	void EstractGold(int GoldAmountToEstract);

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

