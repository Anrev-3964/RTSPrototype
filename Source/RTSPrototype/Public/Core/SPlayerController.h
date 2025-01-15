// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Framework/DataAssets/BuildData.h"
#include "Core/FactionsEnum.h"
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
	void HandleSelection(const TArray<AActor*>& ActorsToSelect);
	void GiveOrders (const FHitResult& HitSelection);

	UFUNCTION()
	void ClearSelected();
	UFUNCTION()
	FHitResult GetMousePositionOnTerrain() const;
	UFUNCTION()
	FHitResult GetMousePositionOnSurface() const;

	//TEnumAsByte<EBuildAssetFilter> GetBuildFilter() const { return BuildAssetFilter; };

	UFUNCTION()
	void MoveUnitsToDestination(const FVector& Destination);

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

	//UPROPERTY()
	//TEnumAsByte<EBuildAssetFilter> BuildAssetFilter = Filter1;

	/** Enhanced Input **/
public:
	UFUNCTION()
	void AddInputMapping(const UInputMappingContext* InputMapping, const int32 MappingPriority = 0) const;
	UFUNCTION()
	void RemoveInputMapping(const UInputMappingContext* InputMapping) const;
	UFUNCTION()
	void SetInputDefault(const bool bEnabled = true) const;
	UFUNCTION()
	void SetInputPlacement(const bool bEnabled = true) const;
	UFUNCTION()
	void SetInputBuildMode(const bool bEnabled = true) const;
	UFUNCTION()
	UDataAsset* GetInputActionAsset() const { return PlayerActionAsset; };

	/** Placement **/

	UFUNCTION()
	bool IsPlacementModeEnabled() const { return bPlacementModeEnabled; };
	UFUNCTION()
	void SetPlacementPreview();
	UFUNCTION()
	void Place();
	UFUNCTION()
	void PlaceCancel();

protected:
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
	UDataAsset* PlayerActionAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* MoveToLocationNiagaraEffect;

	UFUNCTION()
	void ServerPlace(AActor* PlacementPreviewToSpawn);
	UFUNCTION()
	void EndPlacement();
	UFUNCTION()
	void UpdatePlacement() const;

	virtual void Tick(float DeltaTime) override;

	bool bPlacementModeEnabled;

	UPROPERTY()
	AActor* PlacementPreviewActor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Placeable")
	TSubclassOf<AActor> PreviewActorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Other Settings")
	EFaction PlayerFaction;
	//TO DO : definire una funzione che ordini alle unita dentro SelectedActors di muoversi veros la destinazione
};

