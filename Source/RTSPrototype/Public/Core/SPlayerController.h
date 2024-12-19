// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/DataAssets/BuildData.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class UInputMappingContext;
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

	/** Enhanced Input **/
public:
	UFUNCTION()
	void AddInputMapping(const UInputMappingContext* InputMapping, const int32 MappingPriority = 0) const;
	UFUNCTION()
	void RemoveInputMapping(const UInputMappingContext* InputMapping) const;
	UFUNCTION()
	void SetInputDefault(const bool bEnabled = true) const;
	UFUNCTION()
	UDataAsset* GetInputActionAsset() const { return PlayerActionAsset; };

protected:
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
	UDataAsset* PlayerActionAsset;
};
