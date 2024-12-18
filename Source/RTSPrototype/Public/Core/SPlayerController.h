// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/DataAssets/BuildData.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class UBuildComponent;
/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION()
	void HandleSelection(AActor* ActorToSelect);
	void HandleSelection(TArray<AActor*> ActorsToSelect);
	UFUNCTION()
	void ClearSelected();
	UFUNCTION()
	FVector GetMousePositionOnTerrain() const;

	TEnumAsByte<EBuildAssetFilter> GetBuildFilter() const { return BuildAssetFilter; };

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	bool ActorSelected(AActor* ActorToCheck) const;
	UFUNCTION()
	void SelectGroup(const TArray<AActor*>& ActorsToSelect);

	UPROPERTY()
	TArray<AActor*> SelectedActors;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBuildComponent* BuildComponent;

	UPROPERTY()
	TEnumAsByte<EBuildAssetFilter> BuildAssetFilter = Filter1;
};
