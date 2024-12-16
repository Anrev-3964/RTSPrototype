// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SAIController.h"

//dovra ordinare al proprio pawn di muoveris verso la destinaizone usanod un navMEsh

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