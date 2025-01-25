// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AIBehaivorComponent.h"

// Sets default values for this component's properties
UAIBehaivorComponent::UAIBehaivorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAIBehaivorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAIBehaivorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UAIBehaivorComponent::GetPatrolPointsMapLenght()
{
	return PatrolPoints.Num();
}

const AActor* UAIBehaivorComponent::GetPatrolPointActorByIndex(int Index)
 {
 	int CurrentIndex = 0;
 	for (TPair<AActor*, float>& Pair : PatrolPoints)
 	{
 		if (CurrentIndex == Index)
 		{
 			return Pair.Key; // Ritorna un puntatore al TPair
 		}
 		CurrentIndex++;
 	}
 	return nullptr;
 }

float UAIBehaivorComponent::GetPatrolPointWaitTimeByIndex(int Index)
{
	int CurrentIndex = 0;
	for (TPair<AActor*, float>& Pair : PatrolPoints)
	{
		if (CurrentIndex == Index)
		{
			return Pair.Value; // Ritorna un puntatore al TPair
		}
		CurrentIndex++;
	}
	return 0;
}

