// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/FactionsUtils.h"
#include "Core/Selectable.h"
#include "GameFramework/Actor.h"
#include "Framework/DataAssets/BuildData.h"
#include "Buildable.generated.h"

class UBoxComponent;
class UBuildItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildCompleteEvent, const TEnumAsByte<EBuildState>, BuildState);
UCLASS()
class RTSPROTOTYPE_API ABuildable : public AActor,public IFactionsUtils,public ISelectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildable();

	//Data Management
	void Init(UBuildItemDataAsset* BuildItemData, const TEnumAsByte<EBuildState> NewBuildState = EBuildState::NoBuild);
	UBuildItemDataAsset* GetBuildItemData() const {return BuildData;}
	void UpdateOverlayMaterial(const bool bCanPlace = true) const;
	
	FOnBuildCompleteEvent OnBuildCompleteEvent;

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

	UPROPERTY()
	UAssetManager* AssetManager;
	UPROPERTY()
	UBuildItemDataAsset* BuildData;
	UPROPERTY()
	float BuildProgression = 0.0f;
	UPROPERTY()
	FTimerHandle BuildTimer;
	UPROPERTY()
	TEnumAsByte<EBuildState> BuildState = EBuildState::NoBuild;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicOverlayMaterial;

	UPROPERTY()  
	EFaction CurrentFaction = {EFaction::Team1};

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* BoxCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**IFactionUtils Interface**/
	virtual EFaction GetFaction()const override;
	/**End IFactionUtils Interface**/
	void SetCurrentFaction(EFaction NewFaction); //TO DO : add this funciton in the interface

	/** ISelecatbleInterface **/
	UFUNCTION()
	virtual void Select();
	UFUNCTION()
	virtual void DeSelect();
	UFUNCTION()
	virtual void Highlight(const bool Highlight);

};

