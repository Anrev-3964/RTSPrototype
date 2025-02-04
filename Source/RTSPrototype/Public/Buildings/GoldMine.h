// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildable.h"
#include "ITriggerBoxArea.h"
#include "Components/SceneComponent.h"
#include "Core/FactionsUtils.h"
#include "Core/GoldMineData.h"
#include "Core/Selectable.h"
#include "Framework/RTSPlayerState.h"
#include "Framework/DataAssets/BuildData.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GoldMine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMineUpgradedStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMineUpgradedEnd);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMineSelected);
UCLASS()
class RTSPROTOTYPE_API AGoldMine : public AActor,public IFactionsUtils,public ISelectable,public IITriggerBoxArea
{
	GENERATED_BODY()
public:
	AGoldMine(const FObjectInitializer& ObjectInitializer);
	virtual void  BeginPlay() override;
	void SetMineFromDataAsset();

private:
	UPROPERTY(BlueprintReadOnly,Category = "Variables",meta = (AllowPrivateAccess = "true"))  
	EFaction CurrentFaction = {EFaction::Neutral};

	UPROPERTY()  
	EFaction NewOwnerFaction = {EFaction::Neutral};
	
	UPROPERTY(EditDefaultsOnly, Category = "Mine Settings",meta = (Tooltip = "La quantita di oro estraibile dalla miniera (default : 1000)"))
	int16  GoldAmount;
	
	UPROPERTY()
	int CurrentGoldAmount;

	UPROPERTY()
	int GoldEstractAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int  MineCurrentLevel = {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MineCurrentHp = {1.0f};

	UPROPERTY(BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	bool bIsUpgrading = {false};
	UPROPERTY()
	ARTSPlayerState* OwnerPlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	TSoftObjectPtr<UStaticMesh> CompletedMineMesh;

	UPROPERTY()
	TSubclassOf<AActor> TierableActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* RootComponentIntermediate;

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (AllowPrivateAccess = "true"))
	FOnMineSelected OnMineSelect;

public:
	
	UFUNCTION(BlueprintCallable)
	int GetGoldAmount() const ;
	UFUNCTION()
	int GetCurrentGoldAmount() const;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,Category = "Data")
	UGoldMineData* GoldMineData;
	
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

	UFUNCTION(BlueprintCallable)
	void SetStaticMeshFromActor();

	/** ISelecatbleInterface UNUSED FOR NOW**/
	UFUNCTION()
	virtual void Select() override;
	UFUNCTION()
	virtual void DeSelect() override;
	UFUNCTION()
	virtual void Highlight(const bool Highlight) override;
	UFUNCTION(BlueprintCallable)
	virtual void AttackSelectable(const float DamageAmount) override;
	/** ITriggerBoxArea **/
	virtual bool ActorIsATriggerArea() override;

	UPROPERTY(BlueprintAssignable)
	FOnDamageTaken OnDamageTaken;
	FOnBuildStarted OnBuildStarted;
	UPROPERTY(BlueprintAssignable)
	FOnBuildCompleteEvent OnBuildComplete;
	FOnBuildDestroyed OnBuildDestroyed;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;
};

