// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h" 
#include "SAIController.generated.h"

UCLASS()
class RTSPROTOTYPE_API ASAIController : public AAIController
{
	GENERATED_BODY()

	public :

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;
	
	//this values will by passed by units
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
	float UnitSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
	float UnitLoseSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
	float UnitPeripheralVisionAngleDegrees;
	
	ASAIController();
	
	void BeginPLay();

	UFUNCTION()
	void NavigateToDestination(const FVector& Destination);

private:

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
};

