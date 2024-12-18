// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SPlayerController.h"

#include "Buildings/BuildComponent.h"
#include "Core/Selectable.h"
#include "Framework/HUD/CustomHUD.h"

ASPlayerController::ASPlayerController(const FObjectInitializer& ObjectInitializer)
{
	BuildComponent = CreateDefaultSubobject<UBuildComponent>(TEXT("PlayerComponent"));
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

	if (BuildComponent)
	{
		BuildComponent->LoadBuildData();
	}

	if (ACustomHUD* CustomHUD = Cast<ACustomHUD>(GetHUD()))
	{
		CustomHUD->CreateHUD();
	}
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
