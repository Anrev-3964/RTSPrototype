// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Buildings/BuildComponent.h"
#include "Buildings/PlacementPreview.h"
#include "Core/DefaultCameraPawn.h"
#include "Core/Selectable.h"
#include "Framework/DataAssets/PlayerInputActions.h"
#include "Framework/HUD/CustomHUD.h"

ASPlayerController::ASPlayerController(const FObjectInitializer& ObjectInitializer)
{
	BuildComponent = CreateDefaultSubobject<UBuildComponent>(TEXT("PlayerComponent"));
	bPlacementModeEnabled = false;
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

void ASPlayerController::AddInputMapping(const UInputMappingContext* InputMapping, const int32 MappingPriority) const
{
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		ensure(InputMapping);

		if (!InputSubsystem->HasMappingContext(InputMapping))
		{
			InputSubsystem->AddMappingContext(InputMapping, MappingPriority);
		}
	}
}

void ASPlayerController::RemoveInputMapping(const UInputMappingContext* InputMapping) const
{
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		ensure(InputMapping);

		InputSubsystem->RemoveMappingContext(InputMapping);
	}
}

void ASPlayerController::SetInputDefault(const bool bEnabled) const
{
	ensureMsgf(PlayerActionAsset, TEXT("PlayerActionsAsset is NULL"));

	if (const UPlayerInputActions* PlayerActions = Cast<UPlayerInputActions>(PlayerActionAsset))
	{
		ensure(PlayerActions->MappingContextDefault);

		if (bEnabled)
		{
			AddInputMapping(PlayerActions->MappingContextDefault, PlayerActions->MapPriorityDefault);
		}
		else
		{
			RemoveInputMapping(PlayerActions->MappingContextDefault);
		}
	}
}

void ASPlayerController::SetInputPlacement(const bool bEnabled) const
{
	if (const UPlayerInputActions* PlayerActions = Cast<UPlayerInputActions>(PlayerActionAsset))
	{
		ensure(PlayerActions->MappingContextPlacement);

		if (bEnabled)
		{
			AddInputMapping(PlayerActions->MappingContextPlacement, PlayerActions->MapPriorityPlacement);
			//SetInputDefault(!bEnabled); 
		}
		else
		{
			RemoveInputMapping(PlayerActions->MappingContextPlacement);
			//SetInputDefault();
		}
	}
}

void ASPlayerController::SetInputBuildMode(const bool bEnabled) const
{
	if (const UPlayerInputActions* PlayerActions = Cast<UPlayerInputActions>(PlayerActionAsset))
	{
		ensure(PlayerActions->MappingContextBuildMode);

		if (bEnabled)
		{
			AddInputMapping(PlayerActions->MappingContextBuildMode, PlayerActions->MapPriorityBuildMode);
		}
		else
		{
			RemoveInputMapping(PlayerActions->MappingContextBuildMode);
		}
	}
}

void ASPlayerController::SetPlacementPreview()
{
	if (PreviewActorType && !bPlacementModeEnabled)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetMousePositionOnSurface());
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		PlacementPreviewActor = GetWorld()->SpawnActor<APlacementPreview>(
			PreviewActorType, SpawnTransform, SpawnParameters);

		if (PlacementPreviewActor)
		{
			SetInputPlacement();

			bPlacementModeEnabled = true;
		}
	}
}

void ASPlayerController::Place()
{
	if (!IsPlacementModeEnabled() || !PlacementPreviewActor)
	{
		return;
	}

	bPlacementModeEnabled = false;
	SetInputPlacement(false);
	ServerPlace(PlacementPreviewActor);
}

void ASPlayerController::PlaceCancel()
{
	if (!IsPlacementModeEnabled() || !PlacementPreviewActor)
	{
		return;
	}

	bPlacementModeEnabled = false;
	SetInputPlacement(false);
	EndPlacement();
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSubsystem->ClearAllMappings();
		SetInputDefault();
	}
}

void ASPlayerController::ServerPlace(AActor* PlacementPreviewToSpawn)
{
	if (!PlacementPreviewToSpawn)
	{
		return;
	}

	if (const APlacementPreview* Preview = Cast<APlacementPreview>(PlacementPreviewToSpawn))
	{
		FTransform SpawnTransform = Preview->GetActorTransform();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the actual building
		AActor* PlacedBuilding = GetWorld()->SpawnActor<AActor>(
			Preview->PlaceableClass, SpawnTransform, SpawnParameters);

		if (!Preview->PlaceableClass)
		{
			UE_LOG(LogTemp, Error, TEXT("PlaceableClass is null!"));
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Spawning actor of class: %s"), *Preview->PlaceableClass->GetName());

		/*	if (PlacedBuilding)
			{
				for (UActorComponent* Component : PlacedBuilding->GetComponents())
				{
					UE_LOG(LogTemp, Log, TEXT("Component: %s"), *Component->GetName());
				}
				// Log success for debugging
				UE_LOG(LogTemp, Log, TEXT("Building placed successfully at %s"),
				       *PlacedBuilding->GetActorLocation().ToString());
				DrawDebugSphere(GetWorld(), SpawnTransform.GetLocation(), 50.0f, 12, FColor::Red, true, 5.0f);
			}
			else
			{
				// Log failure for debugging
				UE_LOG(LogTemp, Warning, TEXT("Failed to spawn building."));
			}*/
	}

	EndPlacement();
}

void ASPlayerController::EndPlacement()
{
	if (PlacementPreviewActor)
	{
		PlacementPreviewActor->Destroy();
		PlacementPreviewActor = nullptr;
	}

	bPlacementModeEnabled = false;
}

void ASPlayerController::UpdatePlacement() const
{
	if (!PlacementPreviewActor)
	{
		return;
	}

	PlacementPreviewActor->SetActorLocation(GetMousePositionOnSurface());
}

void ASPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPlacementModeEnabled && PlacementPreviewActor)
	{
		UpdatePlacement();
	}
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

FVector ASPlayerController::GetMousePositionOnSurface() const
{
	FVector WorldLocation, WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	FHitResult OutHit;
	if (GetWorld()->LineTraceSingleByChannel(OutHit, WorldLocation,
	                                         WorldLocation + (WorldDirection * 100000.0f), ECC_Visibility))
	{
		if (OutHit.bBlockingHit)
		{
			return OutHit.Location;
		}
	}

	return FVector::ZeroVector;
}
