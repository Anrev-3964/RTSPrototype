// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

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

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	bool ActorSelected(AActor* ActorToCheck) const;
	UFUNCTION()
	void SelectGroup(const TArray<AActor*>& ActorsToSelect);
	
	UPROPERTY()
	TArray<AActor*> SelectedActors;

	//TO DO : definire una funzione che ordini alle unita dentro SelectedActors di muoversi veros la destinazione
};
