// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSPrototypeCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "MaterialDomain.h"
#include "Buildings/GoldMine.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/SAIController.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Framework/DataAssets/CharacterData.h"
#include "Framework/HUD/CustomHUD.h"
#include "Framework/HUD/GameMenuWidget.h"
#include "Framework/HUD/UHudWidget.h"
#include "Kismet/GameplayStatics.h"

ARTSPrototypeCharacter::ARTSPrototypeCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//other components
	ActorAIBehaivorComponent = CreateDefaultSubobject<UAIBehaivorComponent>(TEXT("ActorAIBehaivorComponent"));
}

void ARTSPrototypeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARTSPrototypeCharacter::PlayRandomSound()
{
	if (UnitData->SelectedUnitAudioClips.Num() >0)
	{
		int32 ArrayLenght = UnitData->SelectedUnitAudioClips.Num();
		int32 SelectedAudioClipIndex = FMath::RandRange(0, ArrayLenght-1);

		//Play SelectedSound
		if (USoundBase* SelectedSound = UnitData->SelectedUnitAudioClips[SelectedAudioClipIndex])
		{
			UGameplayStatics::PlaySoundAtLocation(this, SelectedSound, GetActorLocation());
		}
	}
}


void ARTSPrototypeCharacter::BeginPlay()
{
	Super::BeginPlay();
	LoadData();
	AssignUnitStatsFromDataAsset();
	ReceiveBeginPlay(); //far eseguire il begin play del blueprint sempre dopo quello del c++
}

void ARTSPrototypeCharacter::AssignUnitStatsFromDataAsset()
{
	if (!UnitData) return;
	Health = UnitData->GetMaxHealth();
	MaxHealth = UnitData->GetMaxHealth();
	AttackValue = UnitData->GetAttack();
	UnitImage = UnitData->GetImage();
	SpawnDelay = UnitData->GetSpawnDelay();
	AttackMontage = UnitData->GetAttackMontage();
	MiningMontage = UnitData->GetMiningMontage();
	bCanMine = UnitData->GetCanMineGold();
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = UnitData->GetMaxMovementSpeed();
	}
}

void ARTSPrototypeCharacter::ManageBuildMenu(bool bIsSelected)
{
	UE_LOG(LogTemp, Warning, TEXT("Entering ManageBuildMenu"));

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found PlayerController in ManageBuildMenu"));

		ACustomHUD* Hud = Cast<ACustomHUD>(PlayerController->GetHUD());
		if (Hud && Hud->HUD)
		{
			if (Hud->HUD->GameMenuWidget)
			{
				UE_LOG(LogTemp, Warning, TEXT("Build button visibility should change"));
				Hud->HUD->GameMenuWidget->SetVisibility(
					bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Buildbutton is not valid in the GameMenuWidget!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GameMenuWidget is NOT valid"));
			if (Hud)
			{
				UE_LOG(LogTemp, Error, TEXT("HudWidget is valid but GameMenuWidget is NULL"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("HudWidget is NULL"));
			}
		}
	}
}

void ARTSPrototypeCharacter::PlayAudioClip(USoundBase* ClipToPlay)
{
	if (ClipToPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ClipToPlay, GetActorLocation());
	}
}

void ARTSPrototypeCharacter::LoadData() const
{
	if (UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
	{
		if (CharacterDataAssetId.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("Loading Data for Asset ID: %s"), *CharacterDataAssetId.ToString());
			AssetManager->LoadPrimaryAsset(CharacterDataAssetId);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CharacterDataAssetId is not valid!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AssetManager is not initialized!"));
	}
}

UCharacterData* ARTSPrototypeCharacter::GetCharacterData() const
{
	if (const UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
	{
		if (CharacterDataAssetId.IsValid())
		{
			return Cast<UCharacterData>(AssetManager->GetPrimaryAssetObject(CharacterDataAssetId));
		}
	}

	return nullptr;
}

UMaterialInstance* ARTSPrototypeCharacter::GetHighlightMaterial() const
{
	if (const UCharacterData* CharacterData = GetCharacterData())
	{
		const FSoftObjectPath AssetPath = CharacterData->HighlightMaterial.ToSoftObjectPath();
		return Cast<UMaterialInstance>(AssetPath.TryLoad());
	}

	return nullptr;
}

void ARTSPrototypeCharacter::Select()
{
	bSelected = (CurrentFaction == EFaction::Team1);
	
	Highlight(bSelected);
	PlayRandomSound();
	OnUnitSelected.Broadcast(bSelected);

	if (UnitData)
	{
		if (UnitData->GetCanMineGold())
		{
			ManageBuildMenu(bSelected);
			OnPeonSelected.Broadcast(this->GetActorLocation());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unit is not Peone!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterDataAssetId is not valid!"));
	}
}

void ARTSPrototypeCharacter::DeSelect()
{
	bSelected = false;
	Highlight(bSelected);
	OnUnitSelected.Broadcast(bSelected);
	if (UnitData)
	{
		if (UnitData->GetCanMineGold())
		{
			ManageBuildMenu(bSelected);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unit is not Peone!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterDataAssetId is not valid!"));
	}
}

//Prende la/le Mesh e applica l'effetto post process
void ARTSPrototypeCharacter::Highlight(const bool Highlight)
{
TArray<USkeletalMeshComponent*> SkeletalComponents;
GetComponents<USkeletalMeshComponent>(SkeletalComponents);

	for (int32 i = 0; i < SkeletalComponents.Num(); i++)
	{
		if (Highlight)
		{
			if (UMaterialInstance* HighlightMaterial = GetHighlightMaterial())
			{
				if (HighlightMaterial->GetMaterial()->MaterialDomain == MD_PostProcess)
				{
					UE_LOG(LogTemp, Log, TEXT("Applying Custom Depth for post-process material"));

					TArray<UPrimitiveComponent*> PrimitiveComponents;
					GetComponents<UPrimitiveComponent>(PrimitiveComponents);

					for (UPrimitiveComponent* VisualComponent : PrimitiveComponents)
					{
						if (VisualComponent)
						{
							VisualComponent->SetRenderCustomDepth(true);
							UE_LOG(LogTemp, Log, TEXT("Enabled Custom Depth for %s"), *VisualComponent->GetName());
						}
					}
				}
				else
				{
					SkeletalComponents[i]->SetOverlayMaterial(HighlightMaterial);
					UE_LOG(LogTemp, Log, TEXT("Applied Overlay Material: %s to %s"), *HighlightMaterial->GetName(),
					       *SkeletalComponents[i]->GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to get Highlight Material. Falling back."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Disabling highlight"));

			// Disable overlay material
			if (SkeletalComponents[i])
			{
				SkeletalComponents[i]->SetOverlayMaterial(nullptr);
				UE_LOG(LogTemp, Log, TEXT("Removed Overlay Material from %s"), *SkeletalComponents[i]->GetName());
			}

			// Disable custom depth
			TArray<UPrimitiveComponent*> PrimitiveComponents;
			GetComponents<UPrimitiveComponent>(PrimitiveComponents);

			for (UPrimitiveComponent* VisualComponent : PrimitiveComponents)
			{
				if (VisualComponent)
				{
					VisualComponent->SetRenderCustomDepth(false);
					UE_LOG(LogTemp, Log, TEXT("Disabled Custom Depth for %s"), *VisualComponent->GetName());
				}
			}
		}
	}
}

void ARTSPrototypeCharacter::AttackSelectable(const float DamageAmount)
{
	InflictDamage(DamageAmount);
}


//Restituisce il nome della fazione (attualmente inutilizato)
FString ARTSPrototypeCharacter::GetFactionName() const
{
	switch (CurrentFaction)
	{
	case EFaction::Team1:
		return TEXT("Team 1");
	case EFaction::Team2:
		return TEXT("Team 2");
	default:
		return TEXT("Unknown");
	}
}

UBehaviorTree* ARTSPrototypeCharacter::GetBehaviorTree() const
{
	return Tree;
}

UAnimMontage* ARTSPrototypeCharacter::GetAttackMontage() const
{
	return AttackMontage;
}
UAnimMontage* ARTSPrototypeCharacter::GetMiningMontage() const
{
	return MiningMontage;
}


float ARTSPrototypeCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

UUnitData* ARTSPrototypeCharacter::GetUnitData() const
{
	return UnitData;
}

float ARTSPrototypeCharacter::GetHealth() const
{
	return Health;
}

float ARTSPrototypeCharacter::GetSpawnDelay() const
{
	return SpawnDelay;
}

//restituisce "TRUE" se ActorAIBehaivorComponent ha almeno un patrol point
bool ARTSPrototypeCharacter::HasToPatrol() const
{
	if (ActorAIBehaivorComponent)
	{
		if (ActorAIBehaivorComponent->PatrolPoints.Num() > 0)
		{
			return true;
		}
	}
	return false;
}

void ARTSPrototypeCharacter::SetHealth(const float NewHealth)
{
	Health = NewHealth;
}

//Far subire danni all'unita 
void ARTSPrototypeCharacter::InflictDamage(const float Damage)
{
	Health -= Damage;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	//call OnDamageTakenEvent if there is at least 1 subscriber (There is some extra logic in blueprint)
	if (OnDamageTakenEvent.IsBound())
	{
		OnDamageTakenEvent.Broadcast();
	}
	//Health =< 0 :  target death
	if (Health <= 0.f)
	{
		OnDeath.Broadcast();
		DestroyCharacter();
	}
}

void ARTSPrototypeCharacter::DestroyCharacter()
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->UnPossess();
	}
	Destroy();
}

EFaction ARTSPrototypeCharacter::GetFaction() const
{
	return CurrentFaction;
}

//Impostare una nuova fazione per l'unita (attualmente non utilizata)
void ARTSPrototypeCharacter::SetCurrentFaction(EFaction NewFaction)
{
	CurrentFaction = NewFaction;	
}

//Ordinare all'unita di andare a destinazione
void ARTSPrototypeCharacter::MoveToDestination(const FVector Destination)
{
	//get the pawn AI controller
	if (ASAIController* AIController = Cast<ASAIController>(GetController()))
	{
		AIController->NavigateToDestination(Destination);
		OnOrderChanged.Broadcast(EUnitState::MovingToDestination);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController not found for this Pawn!"));
	}
}

//Ordinare all'Unita di attacare (Prende il bersaglio dirretamente dal Behaivor Tree)
void ARTSPrototypeCharacter::Attack()
{
	if (AttackMontage && UnitData)
	{
		float MontageDuration = AttackMontage->GetPlayLength();
		float PlayRate = UnitData->GetAttackSpeed() * MontageDuration;
		PlayAnimMontage(AttackMontage, PlayRate);
		PlayAudioClip(UnitData->UnitAttackAudioClip);
		
		//Try to get the targe from behaivor tree
		if (Tree)
		{
			if (AAIController* AIController = Cast<AAIController>(GetController()))
			{
				if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
				{
					if (UObject* TargetObject = BlackboardComp->GetValueAsObject("TargetActor"))
					{
						if (ISelectable* TargetCharacter = Cast<ISelectable>(TargetObject))
						{
							//Calculate rotation Towards Target
							if (AActor* TargetActor = Cast<AActor>(TargetObject))
							{
								FVector DirectionToTarget = TargetActor->GetActorLocation() - GetActorLocation();
								DirectionToTarget.Z = 0; // Ignora l'altezza per considerare solo l'asse Z

								if (!DirectionToTarget.IsNearlyZero())
								{
									// Calcola la rotazione in base alla direzione
									FRotator LookAtRotation = DirectionToTarget.Rotation();

									// Imposta solo la rotazione lungo l'asse Z (Yaw)
									FRotator NewRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);
									SetActorRotation(NewRotation);
								}
							}
							//Attack Target
							TargetCharacter->AttackSelectable(AttackValue);
							OnAttack.Broadcast();
						}
					} 
				} 
			}
		}
	}
}

//Ordina all'unita di attacare il bersaglio
void ARTSPrototypeCharacter::ChaseTarget(AActor* TargetActor)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "il player ha ordinato di attacare  un nemico");
	}
	if (ASAIController* AIController = Cast<ASAIController>(GetController()))
	{
		AIController->ChaseAndAttackTarget(TargetActor);
		OnOrderChanged.Broadcast(EUnitState::AttackingTarget);
	}
}

//Target : GoldMine'sActor which the unit will reach in order to estract gold
void ARTSPrototypeCharacter::StartMiningGold(AActor* Target)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "l'unita ha ricevuto l'ordine di raccogliere l'oro");
	}
	if (!bCanMine) return;
	if (ASAIController* AIController = Cast<ASAIController>(GetController()))
	{
		AIController->StartMiningGold(Target);
		OnOrderChanged.Broadcast(EUnitState::MiningGold);
	}
}

//quasi identico ad "Attack", con la differenza che interagisce con una miniera
void ARTSPrototypeCharacter::EstractGoldFromMine(AActor* Target)
{
	if (MiningMontage && UnitData)
	{
		float MontageDuration = MiningMontage->GetPlayLength();
		float PlayRate = UnitData->GetMiningSpeed() * MontageDuration;
		PlayAnimMontage(MiningMontage,PlayRate);

		//Try to get the Mine from behaivor tree
		if (Tree)
		{
			if (AAIController* AIController = Cast<AAIController>(GetController()))
			{
				if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
				{
					if (UObject* TargetObject = BlackboardComp->GetValueAsObject("TargetActor"))
					{
						if (AGoldMine* TargetGoldMine = Cast<AGoldMine>(TargetObject))
						{
							OnGoldEstracted.Broadcast(TargetGoldMine->GetGoldEstractionAmount());
							TargetGoldMine->EstractGold();
						}
					}
				}
			}
		}
	} 
}