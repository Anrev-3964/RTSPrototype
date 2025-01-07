// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSPrototypeCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "MaterialDomain.h"
#include "Buildings/GoldMine.h"
#include "RTSPrototypeGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/Button.h"
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
}

void ARTSPrototypeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARTSPrototypeCharacter::BeginPlay()
{
	Super::BeginPlay();
	LoadData();
	AssignUnitStatsFromDataAsset();
}

void ARTSPrototypeCharacter::AssignUnitStatsFromDataAsset()
{
	if (!UnitData)
	{
		return;
	}
	Health = UnitData->GetMaxHealth();
	MaxHealth = UnitData->GetMaxHealth();
	AttackValue = UnitData->GetAttack();
	AttackMontage = UnitData->GetAttackMontage();
	MiningMontage = UnitData->GetMiningMontage();
	CanMineGold = UnitData->GetCanMineGold();
	GoldEstractionCapacity = UnitData->GetGoldEstractionCapacity();
	
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = UnitData->GetMaxMovementSpeed();
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
	bSelected = true;
	Highlight(bSelected);
	if (UnitData)
	{
		if (UnitData->GetName() == TEXT("Peone"))
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

void ARTSPrototypeCharacter::DeSelect()
{
	bSelected = false;
	Highlight(bSelected);
	if (UnitData)
	{
		if (UnitData->GetName() == TEXT("Peone"))
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

void ARTSPrototypeCharacter::SetHealth(const float NewHealth)
{
	Health = NewHealth;
}

void ARTSPrototypeCharacter::InflictDamage(const float Damage)
{
	Health -= Damage;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	//call OnDamageTakenEvent if there is at least 1 subscriber
	if (OnDamageTakenEvent.IsBound())
	{
		OnDamageTakenEvent.Broadcast();
	}
	//Health =< 0 :  target death
	if (Health <= 0.f)
	{
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

void ARTSPrototypeCharacter::SetCurrentFaction(EFaction NewFaction)
{
}

void ARTSPrototypeCharacter::MoveToDestination(const FVector Destination)
{
	//UE_LOG(LogTemp, Warning, TEXT("il pawn chiede al Controller di muoverlo"));
	//get the pawn AI controller
	if (ASAIController* AIController = Cast<ASAIController>(GetController()))
	{
		AIController->NavigateToDestination(Destination);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController not found for this Pawn!"));
	}
}

void ARTSPrototypeCharacter::Attack()
{
	if (AttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("inizio l'animaizone di ATTACO"));
		PlayAnimMontage(AttackMontage);

		//Try to get the targe from behaivor tree
		if (Tree)
		{
			if (AAIController* AIController = Cast<AAIController>(GetController()))
			{
				if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
				{
					if (UObject* TargetObject = BlackboardComp->GetValueAsObject("TargetActor"))
					{
						if (ARTSPrototypeCharacter* TargetCharacter = Cast<ARTSPrototypeCharacter>(TargetObject))
						{
							TargetCharacter->InflictDamage(AttackValue);
						}
					}
				}
			}
		}
	}
}

void ARTSPrototypeCharacter::ChaseTarget(AActor* TargetActor)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "il player ha ordinato di attacare  un nemico");
	}
	if (ASAIController* AIController = Cast<ASAIController>(GetController()))
	{
		AIController->ChaseAndAttackTarget(TargetActor);
	}
}

//Target : GoldMine'sActor which the unit will reach in order to estract gold
void ARTSPrototypeCharacter::StartMiningGold(AActor* Target)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "l'unita ha ricevuto l'ordine di raccogliere l'oro");
	}
	if (!CanMineGold) return;
	if (ASAIController* AIController = Cast<ASAIController>(GetController()))
	{
		AIController->StartMiningGold(Target);
	}
}

void ARTSPrototypeCharacter::EstractGoldFromMine(AActor* Target)
{
	/**
	if (Tree && Target)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (!AIController) return;

		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (!BlackboardComp) return;

		if (UObject* TargetObject = BlackboardComp->GetValueAsObject("TargetActor"))
		{
			if (AGoldMine* TargetGoldMine = Cast<AGoldMine>(TargetObject))
			{
				if (TargetGoldMine->GetGoldAmount() >=0 && MiningMontage)
				{
					PlayAnimMontage(MiningMontage);
					TargetGoldMine->EstractGold();
				}
			}
		}

	}
	**/
	if (MiningMontage)
	{
		PlayAnimMontage(MiningMontage);

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
							//TO DO : modificare questa parte per trasferire l'oro al suo proprietario (azinche al giocatore)
							TargetGoldMine->EstractGold();
						}
					}
				}
			}
		}
	} 
}









