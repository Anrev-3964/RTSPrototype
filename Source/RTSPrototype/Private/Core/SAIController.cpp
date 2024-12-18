// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SAIController.h"

#include "Core/FactionsEnum.h"
#include "Core/FactionsUtils.h"
#include "Perception/AIPerceptionComponent.h"

//dovra ordinare al proprio pawn di muoveris verso la destinaizone usanod un navMEsh

ASAIController::ASAIController()
{
	// Crea il componente di percezione
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSource"));
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

	if (AIPerceptionComponent)
	{
		SightConfig = NewObject<UAISenseConfig_Sight>(this);
		if (SightConfig)
		{
			//TO DO : prendere dirretamente i valori dal blueprint del pawn
			SightConfig->SightRadius = 1000.f;           
			SightConfig->LoseSightRadius = 1200.f;     
			SightConfig->PeripheralVisionAngleDegrees = 180.f; 
			SightConfig->AutoSuccessRangeFromLastSeenLocation = 150.f; 

			// Configura il componente di percezione con il senso visivo
			AIPerceptionComponent->ConfigureSense(*SightConfig);
			PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
			AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
		}
	}
}

void ASAIController::NavigateToDestination(const FVector& Destination)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		// Muovere il pawn verso la destinazione con il navmesh
		MoveToLocation(Destination, -1.0f, true, true, false, true, nullptr, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("this pawn doesent have an AICOntroller"));
	}
}

void ASAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	UE_LOG(LogTemp, Warning, TEXT("Sono dentro la funzione"));
	for (AActor* Actor : UpdatedActors)
	{
		if (Actor)
		{
			if (IFactionsUtils* FactionsUtils = Cast<IFactionsUtils>(Actor))
			{
				if (PawnFaction != FactionsUtils->GetFaction())
				{
					UE_LOG(LogTemp, Warning, TEXT("Ho percepito un attore nemico"));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Ho percepito un attore alleato"));
				}
			}
		}
	}
}