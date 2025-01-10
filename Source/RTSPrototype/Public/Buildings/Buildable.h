// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/FactionsUtils.h"
#include "Core/Selectable.h"
#include "Framework/RTSPlayerState.h"
#include "GameFramework/Actor.h"
#include "Framework/DataAssets/BuildData.h"
#include "Framework/DataAssets/BuildItemDataAsset.h"
#include "Buildable.generated.h"

class UBoxComponent;
class UBuildItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildCompleteEvent, const TEnumAsByte<EBuildState>, BuildState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildStarted,  ABuildable*, BuildableInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildDestroyed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyEventTriggered);
UCLASS()
class RTSPROTOTYPE_API ABuildable : public AActor, public ISelectable, public IFactionsUtils
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildable();

	//Data Management
	UFUNCTION(BlueprintCallable)
	void Init(UBuildItemDataAsset* BuildItemData, const TEnumAsByte<EBuildState> NewBuildState = EBuildState::NoBuild);
	
	UBuildItemDataAsset* GetBuildItemData() const {return BuildData;}
	void UpdateOverlayMaterial(const bool bCanPlace = true) const;
	UMaterialInstance* GetHighlightMaterial() const;

	UPROPERTY(BlueprintAssignable)
	FOnBuildCompleteEvent OnBuildCompleteEvent;
	FOnBuildStarted OnBuildStarted;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMyEventTriggered OnMyEventTriggered;

	virtual EFaction GetFaction() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Settings")
	EFaction CurrentFaction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Build Settings")
	int BuildID;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetBuildingConstructed();
	AActor* GetActor();

	UFUNCTION(BlueprintCallable)
	UBuildItemDataAsset* GetBuildData() const;

	UFUNCTION(BlueprintCallable)
	int GetBuildID() const;

protected:
	
	
	UFUNCTION(BlueprintCallable)
	virtual void Select() override;
	UFUNCTION(BlueprintCallable)
	virtual void DeSelect() override;
	virtual void Highlight(const bool Highlight) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitBuildPreview();
	void StartBuild();
	void EndBuild();
	void UpdateCollider();
	void SetOverlayMaterial();
	void UpdateBuildProgressionMesh();
	void UpdateBuildProgression();
	ARTSPlayerState* GetOwnerPlayerState() const;
	

	/**IFaction Utils **/
	UFUNCTION()
	virtual void SetCurrentFaction(EFaction NewFaction) override;

	UPROPERTY()
	UAssetManager* AssetManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Buildable Data")
	UBuildItemDataAsset* BuildData;

	UPROPERTY()
	TWeakObjectPtr<AActor> WeakActorPtr;

	UPROPERTY()
	float BuildProgression = 0.0f;

	UPROPERTY()
	bool bBuildingConstructed = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="BuildData")
	bool bIsProducingUnit = false;
	UPROPERTY()
	bool bSelected = false;
	
	UPROPERTY()
	FTimerHandle BuildTimer;
	
	UPROPERTY()
	TEnumAsByte<EBuildState> BuildState = EBuildState::NoBuild;

	UPROPERTY()
	EFaction  ECurrentFaction = {EFaction::Neutral};

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicOverlayMaterial;

private:

	UPROPERTY()
	UNiagaraComponent* SpawnedBuildingNiagaraComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	ARTSPlayerState* OwnerPlayerState;
	
};

