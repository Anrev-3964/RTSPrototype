// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SAIController.h"

#include "VectorTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/FactionsEnum.h"
#include "Core/FactionsUtils.h"
#include "Perception/AIPerceptionComponent.h"
#include "RTSPrototype/RTSPrototypeCharacter.h"


ASAIController::ASAIController(FObjectInitializer const& FObjectInitializer)
{
	//AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));	

	/**
	UAISenseConfig_Sight* AISightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	if (AIPerceptionComponent && AISightConfig)
	{
		// Imposta il senso visivo come principale
		AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
		
		AISightConfig->SightRadius = 1000.f; 
		AISightConfig->LoseSightRadius = 1200.f;          
		AISightConfig->PeripheralVisionAngleDegrees = 360.f; 
		AISightConfig->SetMaxAge(1.0f);                    
		
		AIPerceptionComponent->ConfigureSense(*AISightConfig);
        
		AISightConfig->DetectionByAffiliation.bDetectEnemies = true;
		AISightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		AISightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	}
	**/
}

void ASAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Update the Target Variable over time 
	if (GetWorld()->GetTimeSeconds() >(LastInteractionTime + InteractionCooldown))
	{
		Target = FindClosetTarget();
		LastInteractionTime = GetWorld()->GetTimeSeconds();
	}
	if (Target)
	{
		GetBlackboardComponent()->SetValueAsObject("TargetActor", Target);
		if (!GetBlackboardComponent()->GetValueAsBool("HasOrderFromPlayer"))
		{
			GetBlackboardComponent()->SetValueAsVector("TargetLocation",Target->GetActorLocation());
		}
	}
}

void ASAIController::OnPerceptionUpdated(AActor* UpdatedActor, const FAIStimulus Stimulus)
{
	if (UpdatedActor)
	{
		Target =  FindClosetTarget();
		if (Target != nullptr)
		{
			GetBlackboardComponent()->SetValueAsBool("EnemyInSight",Stimulus.WasSuccessfullySensed());
			//UE_LOG(LogTemp, Warning, TEXT("Nemico trovato"));
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Nessun attore nemico"));
	}
}

AActor* ASAIController::FindClosetTarget() const
{
	if (const UAIPerceptionComponent* perceptionComponent = GetAIPerceptionComponent())
	{
		TArray<AActor*> PerceivedActors;
		// Get all perceived Actors
		perceptionComponent->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);
		AActor* ClosestActor = nullptr;
		float ClosestDistance = FLT_MAX; //start value to ensure the first Actor is set as the closest

		for (AActor* Actor : PerceivedActors)
		{
			if (Actor)
			{
				if (IFactionsUtils* FactionsUtils = Cast<IFactionsUtils>(Actor))
				{
					if (PawnFaction != FactionsUtils->GetFaction())
					{
						float ActorDistance = FVector::DistSquared(GetPawn()->GetActorLocation(), Actor->GetActorLocation());

						if (ActorDistance < ClosestDistance)
						{
							ClosestDistance = ActorDistance;
							ClosestActor = Actor;
							//UE_LOG(LogTemp, Error, TEXT("C'E UN NEMICO QUI!"));
						}
					}
				}
			}
		}
		//UE_LOG(LogTemp, Error, TEXT("attore vicino : %p"), ClosestActor);
		return ClosestActor;
	}
	return nullptr;
}

void ASAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	LastInteractionTime = GetWorld()->GetTimeSeconds();
	if (ARTSPrototypeCharacter* const Unit =Cast<ARTSPrototypeCharacter>(InPawn))
	{
		//get the behaivor tree and it's own blackboard and apply it to the AI controller
		if (UBehaviorTree* const Tree = Unit->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			//assign "Tree"'s behaivor tree to AI behaivor tree(it creates one and assign to "b" if it doesent exists)
			UseBlackboard(Tree->BlackboardAsset, b);
			Blackboard = b;
			//start the behaivor tree
			RunBehaviorTree(Tree);
			UE_LOG(LogTemp, Warning, TEXT("Blackboard assigned to %s: %p"), *GetName(), b);
		}
		
		if (APawn* ControlledPawn = GetPawn())
		{
			if (IFactionsUtils* FactionsUtils = Cast<IFactionsUtils>(ControlledPawn))
			{
				PawnFaction = FactionsUtils->GetFaction();
				UE_LOG(LogTemp, Warning, TEXT("Unit Type: %hhd"), PawnFaction);
			}
		}
		
		UnitData = Unit->GetUnitData();

		//Set BlackBoard values
		if (UnitData)
		{
			Blackboard->SetValueAsFloat("AttackDelay",UnitData->GetAttackDelay());
			Blackboard->SetValueAsFloat("AttackRange", UnitData->GetAttackRange());
			Blackboard->SetValueAsFloat("AcceptableRadius", UnitData->GetUnitAcceptableRadius());
		}
	}
}
//mainly called when his pawn get destroyed
void ASAIController::OnUnPossess()
{
	Super::OnUnPossess();
	Destroy();
}



void ASAIController::BeginPLay()
{
	Super::BeginPlay();
}


void ASAIController::NavigateToDestination(const FVector& Destination)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsVector("TargetLocation",Destination);
			BlackboardComponent->SetValueAsBool("HasOrderFromPlayer",true);
			//UE_LOG(LogTemp, Warning, TEXT("NUOVA DESTINAZIONE"));
		}
	}
}

//TO DO : da modificare
void ASAIController::SetUnitSightRadius(int newRadius)
{
	if (SightConfig)
	{
		SightConfig->SightRadius = newRadius;
	}
}
//TO DO : da modificare
void ASAIController::SetUnitLoseSightRadius(int newRadius)
{
	if (SightConfig)
	{
		SightConfig->LoseSightRadius = newRadius;
	}
}



