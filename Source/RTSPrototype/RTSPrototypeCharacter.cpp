// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSPrototypeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/SAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

ARTSPrototypeCharacter::ARTSPrototypeCharacter()
{
	// Default units values
	MovementSpeed = 600.0f;
	
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
	//Set character max velocity
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = MovementSpeed;
	}

	UE_LOG(LogTemp, Warning, TEXT("Faction: %s"), *GetFactionName());
}

void ARTSPrototypeCharacter::Select()
{
	bSelected = true;
	Highlight(bSelected);
}

void ARTSPrototypeCharacter::DeSelect()
{
	bSelected = false;
	Highlight(bSelected);
}

void ARTSPrototypeCharacter::Highlight(const bool Highlight)
{
	TArray<UPrimitiveComponent*> Components;
	GetComponents<UPrimitiveComponent>(Components);
	for (UPrimitiveComponent* VisualComponent : Components)
	{
		if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(VisualComponent))
		{
			Prim->SetRenderCustomDepth(Highlight);
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

void ARTSPrototypeCharacter::MoveToDestination(const FVector Destination)
{
	UE_LOG(LogTemp, Warning, TEXT("il pawn chiede al Controller di muoverlo"));
	//get the pawn AI controller
	if (ASAIController* AIController = Cast<ASAIController>(GetController()))
	{
		AIController->NavigateToDestination(Destination);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController not found for this Pawn!"));
		return;
	}
}




