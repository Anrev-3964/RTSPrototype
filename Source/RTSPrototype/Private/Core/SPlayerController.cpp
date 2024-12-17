// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SPlayerController.h"

#include "Commandable.h"
#include "Core/FactionsUtils.h"
#include "Core/Selectable.h"

ASPlayerController::ASPlayerController(const FObjectInitializer& ObjectInitializer): PlayerFaction()
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
			if (IFactionsUtils* FactionsUtils = Cast<IFactionsUtils>(ActorToSelect))
			{
				if (!FactionsUtils)return;
				
				if (PlayerFaction == FactionsUtils->GetFaction())
				{
					//Selected actor IS in player faction : you can select it
					UE_LOG(LogTemp, Warning, TEXT("Selected Unit is in the same faction of player"));
					Selectable->Select();
					SelectedActors.Add(ActorToSelect);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Selected Unit ISN'T in the same faction of player"));
				}
			}
		}
	}
	else
	{
		ClearSelected();
	}
}

void ASPlayerController::HandleSelection(const TArray<AActor*>& ActorsToSelect)
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
	PlayerFaction = EFaction::Team1;

	if (PlayerFaction != EFaction::Team2)
	{
		UE_LOG(LogTemp, Error, TEXT("PLAYER IS IN A DIFFERENT FACTION:"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PLAYER IS IN THE SAME FACTION:"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Faction: %hhd"), PlayerFaction);
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
				if (IFactionsUtils* FactionsUtils = Cast<IFactionsUtils>(ActorsToSelect[i]))
				{
					if (FactionsUtils &&  PlayerFaction == FactionsUtils->GetFaction())
					{
						UE_LOG(LogTemp, Warning, TEXT("Unit number %d"),i);
						ValidActors.Add(ActorsToSelect[i]);
						Selectable->Select();
					}
				}
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



