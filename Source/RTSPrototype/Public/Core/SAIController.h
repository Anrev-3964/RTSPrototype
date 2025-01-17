// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DetourCrowdAIController.h"
#include "FactionsEnum.h"
#include "UnitStateEnum.h"
#include "Core/UnitData.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h" 
#include "SAIController.generated.h"

UCLASS()
class RTSPROTOTYPE_API ASAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()

	public :
	explicit  ASAIController(FObjectInitializer const& FObjectInitializer);
	
	void BeginPLay();
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;
	

	UFUNCTION()
	void NavigateToDestination(const FVector& Destination);
	UFUNCTION()
	void ChaseAndAttackTarget(AActor* TargetActor);
	void StartMiningGold(AActor* TargetActor);
	
private:

	UPROPERTY()
	float AcceptanceRadius = 50.f;
	
	UPROPERTY()
	EFaction PawnFaction;
	

	/** UnitStateEnum : rappresent current task from player **/
	UPROPERTY()
	EUnitState UnitState; //indica l'ordine/task attuale ricevuto dal giocatore
	UPROPERTY()
	int WaitingForOrder = 0;
	UPROPERTY()
	int MovingToDestination = 1;
	UPROPERTY()
	int AttackingTarget = 2;
	UPROPERTY()
	int MiningGold = 3;
	/** UnitStateEnum : rappresent current task from player **/
	
	UPROPERTY()
	AActor* Target;

	UPROPERTY()
	UUnitData* UnitData;
	
	UFUNCTION(BlueprintCallable, Category = "AI Perception")
	void OnPerceptionUpdated(AActor* UpdatedActor, const FAIStimulus Stimulus);
	UFUNCTION()
	AActor* FindClosetTarget() const ;
	UFUNCTION()
	void HandleCurrentOrder();

	float LastInteractionTime;
	float InteractionCooldown = 0.1f;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual  void OnUnPossess() override;
};




