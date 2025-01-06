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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyEventTriggered);

UCLASS()
class RTSPROTOTYPE_API ABuildable : public AActor, public ISelectable, public IFactionsUtils
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildable();

	//Data Management
	void Init(UBuildItemDataAsset* BuildItemData, const TEnumAsByte<EBuildState> NewBuildState = EBuildState::NoBuild);
	UBuildItemDataAsset* GetBuildItemData() const {return BuildData;}
	void UpdateOverlayMaterial(const bool bCanPlace = true) const;
	UMaterialInstance* GetHighlightMaterial() const;
	
	FOnBuildCompleteEvent OnBuildCompleteEvent;

	virtual EFaction GetFaction() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Settings")
	EFaction CurrentFaction;

protected:

	// Delegato esposto a Blueprint
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMyEventTriggered OnMyEventTriggered;
	
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

	UPROPERTY()
	UAssetManager* AssetManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Buildable Data")
	UBuildItemDataAsset* BuildData;
	UPROPERTY()
	float BuildProgression = 0.0f;
	UPROPERTY()
	FTimerHandle BuildTimer;
	UPROPERTY()
	TEnumAsByte<EBuildState> BuildState = EBuildState::NoBuild;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicOverlayMaterial;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* BoxCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;

	bool bSelected;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	UBuildItemDataAsset* GetBuildData() const;

};

