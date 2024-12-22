// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FactionsEnum.h"
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

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
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
	AActor* Target;
	
	UFUNCTION(BlueprintCallable, Category = "AI Perception")
	void OnPerceptionUpdated(AActor* UpdatedActor, const FAIStimulus Stimulus);
	UFUNCTION(BlueprintCallable, Category = "AI Perception")
	AActor* FindClosetTarget() const ;
	
	UFUNCTION()
	void SetUpUnitPerceptionComponent();

	UFUNCTION()
	void CheckIfEnemyIsClose();
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual  void OnUnPossess() override;
};




