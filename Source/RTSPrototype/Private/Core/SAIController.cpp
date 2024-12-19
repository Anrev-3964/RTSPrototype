// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SAIController.h"

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
		if (IFactionsUtils* FactionsUtils = Cast<IFactionsUtils>(UpdatedActor))
		{
			//check if the stimulus is an enemy
			if (PawnFaction != FactionsUtils->GetFaction())
			{
				UE_LOG(LogTemp, Warning, TEXT("Ho percepito un attore nemico"));
				GetBlackboardComponent()->SetValueAsBool("EnemyInSight",Stimulus.WasSuccessfullySensed());
				Target = UpdatedActor;
				
				//TO DO : passare alla blackBoard il bersaglio
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Ho percepito un attore alleato"));
			}
		}		
	}
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
	}
}


void ASAIController::BeginPLay()
{
	Super::BeginPlay();
	
	
	if (APawn* ControlledPawn = GetPawn())
	{
		if (IFactionsUtils* FactionsUtils = Cast<IFactionsUtils>(ControlledPawn))
		{
			PawnFaction = FactionsUtils->GetFaction();
		}
	}
}


void ASAIController::NavigateToDestination(const FVector& Destination)
{
	//questa funzione viene richiamata dal player
	//TO DO : muovere l'unita modificanod la TargetLocaation nel Behaivor tree 


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
	
	/**
	if (APawn* ControlledPawn = GetPawn())
	{
		// Muovere il pawn verso la destinatione con il navmesh
		MoveToLocation(Destination, -1.f, true, true, false, true, nullptr, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("this pawn doesent have an AICOntroller"));
	}
	**/
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



