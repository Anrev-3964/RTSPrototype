// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/SPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Buildings/BuildComponent.h"
#include "Buildings/PlacementPreview.h"
#include "Core/DefaultCameraPawn.h"

#include "Commandable.h"
#include "NiagaraFunctionLibrary.h"
#include "Buildings/GoldMine.h"
#include "Core/FactionsUtils.h"
#include "Core/Selectable.h"
#include "Framework/DataAssets/PlayerInputActions.h"
#include "Framework/HUD/CustomHUD.h"
#include "RTSPrototype/RTSPrototypeCharacter.h"

ASPlayerController::ASPlayerController(const FObjectInitializer& ObjectInitializer): PlayerFaction()
{
	BuildComponent = CreateDefaultSubobject<UBuildComponent>(TEXT("PlayerComponent"));
	bPlacementModeEnabled = false;
}

//Selezionare le unita
void ASPlayerController::HandleSelection(AActor* ActorToSelect)
{
	if (ISelectable* Selectable = Cast<ISelectable>(ActorToSelect))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor implements ISelectable: %s"), *ActorToSelect->GetName());

		if (ActorSelected(ActorToSelect))
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor is already selected. Deselecting: %s"), *ActorToSelect->GetName());
			Selectable->DeSelect();
			SelectedActors.Remove(ActorToSelect);
		}
		else
		{
			if (IFactionsUtils* FactionsUtils = Cast<IFactionsUtils>(ActorToSelect))
			{
				if (FactionsUtils->GetFaction() == PlayerFaction)
				{
					//Selected actor IS in player faction : you can select it
					/**
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "il player ha selezionato un alleato");
					}
					**/
					Selectable->Select();
					SelectedActors.Add(ActorToSelect);
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

//ordinare alle unita di fare qualcosa a seconda di cosa clicchi
void ASPlayerController::GiveOrders(const FHitResult& HitSelection)
{
	if (SelectedActors.Num() <=0) return;
	
	AActor* ActorToSelect = HitSelection.GetActor();
	if (!ActorToSelect) return;

	//**Player Selected something selectable **//
	if (ISelectable* Selectable = Cast<ISelectable>(ActorToSelect))
	{
		//** Check if Selected Actor is in the same player Faction**//
		if (IFactionsUtils* FactionsUtils = Cast<IFactionsUtils>(ActorToSelect))
		{
			if (!FactionsUtils)return;

			EFaction TargetFaction = FactionsUtils->GetFaction();
			if (TargetFaction == EFaction::Neutral)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "Neutrale selezionato");
				}
				return;
			} 

			//** Selected Actor isnt in same player Faction**//
			if (PlayerFaction != TargetFaction) //Selected actor IS in player faction : you can select it
			{
				if (SelectedActors.Num() <= 0) return;
				for (AActor* Actor : SelectedActors)
				{
					if (Actor)
					{
						if (ICommandable* CommandableActor = Cast<ICommandable>(Actor))
						{
							CommandableActor->ChaseTarget(ActorToSelect);
						}
					}
				}
			}
			//** Selected Actor is in same player Faction**//
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, "il player ha selezionato un alleato");
				}
				if (AGoldMine* SelectedBuilding = Cast<AGoldMine>(ActorToSelect))// Selecte Actor is a Gold Mine
				{
					if (SelectedActors.Num() <= 0) return;

					for (AActor* Actor : SelectedActors)
					{
						if (Actor)
						{
							if (ICommandable* CommandableActor = Cast<ICommandable>(Actor))
							{
								CommandableActor->StartMiningGold(ActorToSelect);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		MoveUnitsToDestination(HitSelection.ImpactPoint);
		//SpawnNiagaraEffect
		if (MoveToLocationNiagaraEffect)
		{
			//Spawn Niagara Effect
			FVector Location = HitSelection.ImpactPoint;
			FRotator Rotation = FRotator::ZeroRotator;

			UNiagaraComponent* SpawnedBuildingNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),         // World
				MoveToLocationNiagaraEffect,      // Niagara System To Spawn
				Location,           // Position
				Rotation,           // Rotation
				FVector(1.5f)); // Scale
		}
	}
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
	PlayerFaction = EFaction::Team1;
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
		SpawnTransform.SetLocation(GetMousePositionOnSurface().Location);
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

	PlacementPreviewActor->SetActorLocation(GetMousePositionOnSurface().Location);
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

FHitResult ASPlayerController::GetMousePositionOnTerrain() const
{
	FVector WorldLocation, WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	FHitResult OutHit;
	if (GetWorld()->LineTraceSingleByChannel(OutHit, WorldLocation,
		WorldLocation + (WorldDirection * 100000.0f), ECC_GameTraceChannel1))
	{
		if (OutHit.bBlockingHit)
		{
			ARTSPrototypeCharacter* Unit = Cast<ARTSPrototypeCharacter>(OutHit.GetActor());
			return OutHit;
		}
	}

	return FHitResult();
}

FHitResult ASPlayerController::GetMousePositionOnSurface() const
{
	FVector WorldLocation, WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	FHitResult OutHit;
	if (GetWorld()->LineTraceSingleByChannel(OutHit, WorldLocation,
											 WorldLocation + (WorldDirection * 100000.0f), ECC_Visibility))
	{
		if (OutHit.bBlockingHit)
		{
			return OutHit;
		}
	}
	return FHitResult();
}

void ASPlayerController::MoveUnitsToDestination(const FVector& Destination)
{
	if (SelectedActors.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("non ho attori da muovere"));
		return;
	}
	for (AActor* Actor : SelectedActors)
	{
		UE_LOG(LogTemp, Error, TEXT("Ho almeno un attore, destinazione: %s"), *Destination.ToString());
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



