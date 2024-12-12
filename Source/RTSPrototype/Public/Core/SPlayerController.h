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

	UFUNCTION()
	void ClearSelected();

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	bool ActorSelected(AActor* ActorToCheck) const;

	

	UPROPERTY()
	TArray<AActor*> SelectedActors;
};
