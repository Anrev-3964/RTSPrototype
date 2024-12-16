// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSPrototypeCharacter.h"

#include "MaterialDomain.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Framework/DataAssets/CharacterData.h"

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
}

void ARTSPrototypeCharacter::DeSelect()
{
	bSelected = false;
	Highlight(bSelected);
}

void ARTSPrototypeCharacter::Highlight(const bool Highlight)
{
	/*TArray<UPrimitiveComponent*> Components;
	GetComponents<UPrimitiveComponent>(Components);
	for (UPrimitiveComponent* VisualComponent : Components)
	{
		if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(VisualComponent))
		{
			Prim->SetRenderCustomDepth(Highlight);
		}
	}*/

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
                UE_LOG(LogTemp, Log, TEXT("Applied Overlay Material: %s to %s"), *HighlightMaterial->GetName(), *SkeletalComponents[i]->GetName());
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
