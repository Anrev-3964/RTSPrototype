// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FactionsEnum.h"
#include "UnitStateEnum.h"
#include "Core/UnitData.h"
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
	explicit  ASAIController(FObjectInitializer const& FObjectInitializer);
	
	void BeginPLay();
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;
	

	UFUNCTION()
	void NavigateToDestination(const FVector& Destination);
	UFUNCTION()
	void SetUnitSightRadius(int newRadius);
	void SetUnitLoseSightRadius(int newRadius);
	
private:

	UPROPERTY()
	float AcceptanceRadius = 50.f;
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY()
	EFaction PawnFaction;
	
	UPROPERTY()
	EUnitState UnitState; //indica l'ordine/task attuale ricevuto dal giocatore

	UPROPERTY()
	AActor* Target;

	UPROPERTY()
	UUnitData* UnitData;
	
	UFUNCTION(BlueprintCallable, Category = "AI Perception")
	void OnPerceptionUpdated(AActor* UpdatedActor, const FAIStimulus Stimulus);
	UFUNCTION()
	AActor* FindClosetTarget() const ;

	float LastInteractionTime;
	float InteractionCooldown = 0.8f;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual  void OnUnPossess() override;
};




