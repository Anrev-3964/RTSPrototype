// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SAIController.h"

#include "VectorTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/FactionsEnum.h"
#include "Core/FactionsUtils.h"
#include "Perception/AIPerceptionComponent.h"
#include "RTSPrototype/RTSPrototypeCharacter.h"

//dovra ordinare al proprio pawn di muoveris verso la destinaizone usanod un navMEsh


ASAIController::ASAIController(FObjectInitializer const& FObjectInitializer)
{
	SetUpUnitPerceptionComponent();
}

void ASAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Target)
	{
		GetBlackboardComponent()->SetValueAsObject("TargetActor", Target);
		if (!GetBlackboardComponent()->GetValueAsBool("HasOrderFromPlayer"))
		{
			GetBlackboardComponent()->SetValueAsVector("TargetLocation",Target->GetActorLocation());
		}
	}
}

void ASAIController::SetUpUnitPerceptionComponent()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSource"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	ARTSPrototypeCharacter* ControlledPawn = Cast<ARTSPrototypeCharacter>(GetPawn());
	
	if (SightConfig && ControlledPawn)
	{
		SetPerceptionComponent(*AIPerceptionComponent);
			
		SightConfig->SightRadius = ControlledPawn->GetUnitSightRadius();           
		SightConfig->LoseSightRadius = ControlledPawn->GetUnitLoseSightRadius();     
		SightConfig->PeripheralVisionAngleDegrees = 360.f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 500.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		// Configura il componente di percezione con il senso visivo

		GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
		//GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,&ASAIController::OnPerceptionUpdated);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		UE_LOG(LogTemp, Error, TEXT("SetUp finito con successo"));
	}
}

void ASAIController::CheckIfEnemyIsClose()
{
	if (!Target)
		return;

	FVector AIPawnPosition = GetPawn()->GetActorLocation();
	FVector EnemyPosition = Target->GetActorLocation();

	float Distance = FVector::Dist(AIPawnPosition, EnemyPosition);

	if (Distance <= AcceptanceRadius)
	{
		GetBlackboardComponent()->SetValueAsBool("CanAttackEnemy",true);
	}
	else
	{
		GetBlackboardComponent()->SetValueAsBool("CanAttackEnemy",false);
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
			UE_LOG(LogTemp, Warning, TEXT("Nemico trovato"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nessun attore nemico"));
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
						}
					}
				}
			}
		}
		return ClosestActor;
	}
	return nullptr;
}



void ASAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
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
		}
		if (APawn* ControlledPawn = GetPawn())
		{
			if (IFactionsUtils* FactionsUtils = Cast<IFactionsUtils>(ControlledPawn))
			{
				PawnFaction = FactionsUtils->GetFaction();
				UE_LOG(LogTemp, Warning, TEXT("Unit Type: %hhd"), PawnFaction);
			}
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
			UE_LOG(LogTemp, Warning, TEXT("NUOVA DESTINAZIONE"));
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("this pawn doesent have an AICOntroller"));
	}
}

void ASAIController::SetUnitSightRadius(int newRadius)
{
	if (SightConfig)
	{
		SightConfig->SightRadius = newRadius;
	}
}

void ASAIController::SetUnitLoseSightRadius(int newRadius)
{
	if (SightConfig)
	{
		SightConfig->LoseSightRadius = newRadius;
	}
}



