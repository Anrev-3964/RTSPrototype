// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SPlayerController.h"

#include "Commandable.h"
#include "Core/Selectable.h"

ASPlayerController::ASPlayerController(const FObjectInitializer& ObjectInitializer)
{
}

void ASPlayerController::HandleSelection(AActor* ActorToSelect)
{
	if (ISelectable* Selectable = Cast<ISelectable>(ActorToSelect))
	{
		if (ActorToSelect && ActorSelected(ActorToSelect))
		{
			Selectable->DeSelect();
			SelectedActors.Remove(ActorToSelect);
		}
		else
		{
			Selectable->Select();
			SelectedActors.Add(ActorToSelect);
		}
	}
	else
	{
		ClearSelected();
	}
}

void ASPlayerController::HandleSelection(TArray<AActor*> ActorsToSelect)
{
	SelectGroup(ActorsToSelect);
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

bool ASPlayerController::ActorSelected(AActor* ActorToCheck) const
{
	return SelectedActors.Contains(ActorToCheck);
}

void ASPlayerController::SelectGroup(const TArray<AActor*>& ActorsToSelect)
{
	ClearSelected();
	TArray<AActor*> ValidActors;
	for (int i = 0; i < ActorsToSelect.Num(); i++)
	{
		if (ActorsToSelect[i])
		{
			if (ISelectable* Selectable = Cast<ISelectable>(ActorsToSelect[i]))
			{
				ValidActors.Add(ActorsToSelect[i]);
				Selectable->Select();
			}
		}
	}
	
	SelectedActors.Append(ValidActors);
	ValidActors.Empty();
}

void ASPlayerController::ClearSelected()
{
	for (AActor* SelectedActor : SelectedActors)
	{
		if (SelectedActor)
		{
			if (ISelectable* Selectable = Cast<ISelectable>(SelectedActor))
			{
				Selectable->DeSelect();
			}
		}
	}

	SelectedActors.Empty();
}

FVector ASPlayerController::GetMousePositionOnTerrain() const
{
	FVector WorldLocation, WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	FHitResult OutHit;
	if (GetWorld()->LineTraceSingleByChannel(OutHit, WorldLocation,
		WorldLocation + (WorldDirection * 100000.0f), ECC_GameTraceChannel1))
	{
		if (OutHit.bBlockingHit)
		{
				return OutHit.Location;
		}
	}

	return FVector::ZeroVector;
}

void ASPlayerController::MoveUnitsToDestination(const FVector& Destination)
{
	for (AActor* Actor : SelectedActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ho almeno un attore, destinazione: %s"), *Destination.ToString());
		if (Actor)
		{
			if (ICommandable* CommandableActor = Cast<ICommandable>(Actor))
			{
				UE_LOG(LogTemp, Warning, TEXT("ordino al pawn di muoversi"));
				CommandableActor->MoveToDestination(Destination);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor %s does not implement ICommandable interface!"), *Actor->GetName());
		}
	}
}



