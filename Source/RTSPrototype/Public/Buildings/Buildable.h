// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/FactionsUtils.h"
#include "Core/Selectable.h"
#include "Framework/RTSPlayerState.h"
#include "GameFramework/Actor.h"
#include "Framework/DataAssets/BuildData.h"
#include "Buildable.generated.h"

class UBoxComponent;
class UBuildItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildCompleteEvent, const TEnumAsByte<EBuildState>, BuildState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildStarted,  ABuildable*, BuildableInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildDestroyed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyEventTriggered);
UCLASS()
class RTSPROTOTYPE_API ABuildable : public AActor,public IFactionsUtils, public ISelectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildable();

	//Data Management
	void Init(UBuildItemDataAsset* BuildItemData, const TEnumAsByte<EBuildState> NewBuildState = EBuildState::NoBuild);
	UBuildItemDataAsset* GetBuildItemData() const {return BuildData;}
	void UpdateOverlayMaterial(const bool bCanPlace = true) const;

	UPROPERTY(BlueprintAssignable)
	FOnBuildCompleteEvent OnBuildCompleteEvent;
	FOnBuildStarted OnBuildStarted;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMyEventTriggered OnMyEventTriggered;

protected:
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
	UBuildItemDataAsset* GetBuildData() const;
	UMaterialInstance* GetHighlightMaterial() const;

	/**ISelectable **/
	void Select();
	void DeSelect();
	void Highlight(bool Highlight);

	/**IFaction Utils **/
	UFUNCTION()
	virtual void SetCurrentFaction(EFaction NewFaction) override;

	UPROPERTY()
	UAssetManager* AssetManager;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="BuildData")
	UBuildItemDataAsset* BuildData;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	ARTSPlayerState* OwnerPlayerState;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool GetBuildingConstructed();

	/**IFaction Utils **/
	UFUNCTION()
	virtual EFaction GetFaction()const override;
	
};

