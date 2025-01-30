// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TrainingComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UTrainingComponent::UTrainingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UTrainingComponent::BeginPlay()
{
	Super::BeginPlay();
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 5.0f;
}


// Called every frame
void UTrainingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RemoveDeadUnits();
	if (TrainedUnits.Num() < MaxUnits && !bIsTraining)
	{
		TrainNewUnits();
	}
	
	//FString Message = FString::Printf(TEXT("Unita ancora vive è: %d"), TrainedUnits.Num());
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, Message);
}

void UTrainingComponent::RemoveDeadUnits()
{
	TrainedUnits.RemoveAll([](ARTSPrototypeCharacter* Unit)
	{
		return !IsValid(Unit);  // Rimuovi se l'unità non è valida
	});
}

void UTrainingComponent::TrainNewUnits()
{
	if (TrainableUnits.Num() == 0 || bIsTraining) return;

	bIsTraining = true;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Inizia l'addestramento");
	int32 RandomIndex = FMath::RandRange(0, TrainableUnits.Num() - 1);

	TSubclassOf<ARTSPrototypeCharacter> UnitToSpawn = TrainableUnits[RandomIndex];
	float TrainingTime = 5.0f;
	
	GetWorld()->GetTimerManager().SetTimer(TrainingTimerHandle, [this, UnitToSpawn]
	{
		SpawnUnit(UnitToSpawn); // Passa il parametro UnitToSpawn alla funzione
	}, TrainingTime, false);
}

void UTrainingComponent::SpawnUnit(TSubclassOf<ARTSPrototypeCharacter> UnitToSpawn)
{
	if (!UnitToSpawn) return;
	GetWorld()->GetTimerManager().ClearTimer(TrainingTimerHandle);

	// Spawn l'attore nel mondo
	ARTSPrototypeCharacter* SpawnedUnit = GetWorld()->SpawnActor<ARTSPrototypeCharacter>(UnitToSpawn, UnitSpawnLocation,UnitSpawnRotation);

	if (!SpawnedUnit) return;
	TrainedUnits.Add(SpawnedUnit);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, "Spawno l'unita");
	
	bIsTraining = false;
	if (TrainedUnits.Num() < MaxUnits)
		{
			TrainNewUnits();
		}
}

