// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SPlayerController.h"

#include "LightmapResRatioAdjust.h"
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



