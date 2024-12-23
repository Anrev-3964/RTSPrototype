// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DefaultCameraPawn.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Buildings/BuildComponent.h"
#include "Camera/CameraComponent.h"
#include "Core/SelectionBox.h"
#include "Framework/DataAssets/PlayerInputActions.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADefaultCameraPawn::ADefaultCameraPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("DefaultCameraPawn");
	RootComponent = SceneComponent;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 2000.f;
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ADefaultCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	TargetLocation = GetActorLocation();
	TargetZoom = 3000.0f;

	const FRotator Rotation = SpringArm->GetRelativeRotation();
	TargetRotation = FRotator(Rotation.Pitch - 50, Rotation.Yaw, 0);

	SPlayer = Cast<ASPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (SPlayer)
	{
		BuildComponent = UBuildComponent::FindBuildComponent(SPlayer);
	}

	CreateSelectionBox();
}


void ADefaultCameraPawn::EnableRotate()
{
	bCanRotate = true;
}

void ADefaultCameraPawn::DisableRotate()
{
	bCanRotate = false;
}

void ADefaultCameraPawn::RotateHorizontal(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}

	if (bCanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0, AxisValue, 0));
	}
}

void ADefaultCameraPawn::RotateVertical(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}

	if (bCanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(AxisValue, 0, 0));
	}
}

void ADefaultCameraPawn::CameraBounds()
{
	float NewPitch = TargetRotation.Pitch;
	if (TargetRotation.Pitch < (RotationPitchMax * -1))
	{
		NewPitch = (RotationPitchMax * -1);
	}

	else if (TargetRotation.Pitch > (RotationPitchMin * -1))
	{
		NewPitch = (RotationPitchMin * -1);
	}

	TargetRotation = FRotator(NewPitch, TargetRotation.Yaw, 0);
}

AActor* ADefaultCameraPawn::GetSelectedObject()
{
	if (UWorld* World = GetWorld())
	{
		FVector WorldLocation, WorldDirection;
		SPlayer->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		FVector EndLocation = WorldDirection * 1000000.0f + WorldLocation;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FHitResult Hit;
		if (World->LineTraceSingleByChannel(Hit, WorldLocation, EndLocation, ECC_Visibility, Params))
		{
			DrawDebugLine(World, WorldLocation, Hit.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.0f);
			DrawDebugSphere(World, Hit.ImpactPoint, 10.0f, 12, FColor::Red, false, 2.0f);

			if (AActor* Actor = Hit.GetActor())
			{
				return Actor;
			}
		}
		else
		{
			// Debug visuals for a missed raycast
			DrawDebugLine(World, WorldLocation, EndLocation, FColor::Red, false, 2.0f, 0, 1.0f);
		}
	}

	return nullptr;
}

void ADefaultCameraPawn::CreateSelectionBox()
{
	if (!SelectionBoxClass)
	{
		return;
	}
	if (UWorld* WorldContext = GetWorld())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = this;
		SelectionBox = WorldContext->SpawnActor<ASelectionBox>(SelectionBoxClass, FVector::ZeroVector,
		                                                       FRotator::ZeroRotator, SpawnParameters);
		if (SelectionBox)
		{
			SelectionBox->SetOwner(this);
		}
	}
}

void ADefaultCameraPawn::Move(const FInputActionValue& Value)
{
	if (!SpringArm)
	{
		return;
	}
	if (ensure(Value.GetValueType() == EInputActionValueType::Axis2D))
	{
		TargetLocation += SpringArm->GetTargetRotation().RotateVector(Value.Get<FVector>()) * MovementSpeed;
		GetTerrainPosition(TargetLocation);
	}
}

void ADefaultCameraPawn::EdgeScroll()
{
	if (UWorld* WorldContext = GetWorld())
	{
		FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(WorldContext);
		const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(WorldContext);
		MousePosition = MousePosition * UWidgetLayoutLibrary::GetViewportScale(WorldContext);
		MousePosition.X = MousePosition.X / ViewportSize.X;
		MousePosition.Y = MousePosition.Y / ViewportSize.Y;

		//right/left
		if (MousePosition.X > 0.98f && MousePosition.X < 1.0f)
		{
			TargetLocation += SpringArm->GetTargetRotation().RotateVector(FVector(0, 1, 0)) * EdgeScrollSpeed;
		}
		else if (MousePosition.X < 0.02f && MousePosition.X > 0.0f)
		{
			TargetLocation += SpringArm->GetTargetRotation().RotateVector(FVector(0, -1, 0)) * EdgeScrollSpeed;
		}

		//forward/backward
		if (MousePosition.Y > 0.98f && MousePosition.Y < 1.0f)
		{
			TargetLocation += SpringArm->GetTargetRotation().RotateVector(FVector(-1, 0, 0)) * EdgeScrollSpeed;
		}
		else if (MousePosition.Y < 0.02f && MousePosition.Y > 0.0f)
		{
			TargetLocation += SpringArm->GetTargetRotation().RotateVector(FVector(1, 0, 0)) * EdgeScrollSpeed;
		}

		GetTerrainPosition(TargetLocation);
	}
}

void ADefaultCameraPawn::GetTerrainPosition(FVector& TerrainPosition) const
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	FVector TraceOrigin = TerrainPosition;
	TraceOrigin.Z += 10000.0f;
	FVector TraceEnd = TerrainPosition;
	TraceEnd.Z -= 10000.0f;

	if (UWorld* WorldContext = GetWorld())
	{
		if (WorldContext->LineTraceSingleByChannel(Hit, TraceOrigin, TraceEnd, ECC_Visibility, Params))
		{
			TerrainPosition = Hit.ImpactPoint;
		}
	}
}

void ADefaultCameraPawn::Look(const FInputActionValue& Value)
{
	if (ensure(Value.GetValueType() == EInputActionValueType::Axis1D))
	{
		const float NewPitch = Value.Get<float>() * RotationSpeed * 0.25f;
		TargetRotation = FRotator(TargetRotation.Pitch + NewPitch, TargetRotation.Yaw, 0);
		if (bCanRotate)
		{
			TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0, NewPitch, 0));
		}
	}
}

void ADefaultCameraPawn::Rotate(const FInputActionValue& Value)
{
	if (ensure(Value.GetValueType() == EInputActionValueType::Axis1D))
	{
		const float NewRotation = Value.Get<float>() * RotationSpeed;
		TargetRotation = FRotator(TargetRotation.Pitch, TargetRotation.Yaw + NewRotation, 0);
	}
}

void ADefaultCameraPawn::Zoom(const FInputActionValue& Value)
{
	if (ensure(Value.GetValueType() == EInputActionValueType::Axis1D))
	{
		TargetZoom = FMath::Clamp(TargetZoom + (Value.Get<float>() * ZoomSpeed), ZoomMin, ZoomMax);
	}
}

void ADefaultCameraPawn::Select(const FInputActionValue& Value)
{
	if (!SPlayer)
	{
		return;
	}

	SPlayer->HandleSelection(nullptr);
	bBoxSelected = false;
	SelectHitLocation = SPlayer->GetMousePositionOnTerrain();
}

void ADefaultCameraPawn::SelectHold(const FInputActionValue& Value)
{
	if (!SPlayer)
	{
		return;
	}

	if ((SelectHitLocation - SPlayer->GetMousePositionOnTerrain()).Length() > 100.0f)
	{
		if (!bBoxSelected && SelectionBox)
		{
			SelectionBox->Start(SelectHitLocation, TargetRotation);
			bBoxSelected = true;
		}
	}
}

void ADefaultCameraPawn::SelectEnd(const FInputActionValue& Value)
{
	if (!SPlayer)
	{
		return;
	}

	if (bBoxSelected && SelectionBox)
	{
		SelectionBox->End();
		bBoxSelected = false;
	}
	else
	{
		//if no box selection perform a single selection
		SPlayer->HandleSelection(GetSelectedObject());
	}
}

void ADefaultCameraPawn::TestPlacement(const FInputActionValue& Value)
{
	if (!SPlayer)
	{
		return;
	}
	SPlayer->SetPlacementPreview();
}

void ADefaultCameraPawn::Place(const FInputActionValue& Value)
{
	if (!SPlayer)
	{
		return;
	}
	if (SPlayer->IsPlacementModeEnabled())
	{
		SPlayer->Place();
	}
}

void ADefaultCameraPawn::PlaceCancel(const FInputActionValue& Value)
{
	if (!SPlayer)
	{
		return;
	}
	if (SPlayer->IsPlacementModeEnabled())
	{
		SPlayer->PlaceCancel();
	}
}

void ADefaultCameraPawn::BuildDeploy(const FInputActionValue& Value)
{
	if (!BuildComponent)
	{
		return;
	}

	BuildComponent->BuildingDeploy();
}

void ADefaultCameraPawn::BuildCancel(const FInputActionValue& Value)
{
	if (!BuildComponent)
	{
		return;
	}

	BuildComponent->ExitBuildMode();
}

// Called every frame
void ADefaultCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EdgeScroll();
	CameraBounds();

	const FVector InterpolatedLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime,
	                                                                   MovementSpeed);
	SetActorLocation(InterpolatedLocation);

	const float InterpolatedZoom = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, TargetZoom, DeltaTime,
	                                                             ZoomSpeed);
	SpringArm->TargetArmLength = InterpolatedZoom;

	const FRotator InterpolatedRotation = UKismetMathLibrary::RInterpTo(SpringArm->GetRelativeRotation(),
	                                                                    TargetRotation, DeltaTime, RotationSpeed);
	SpringArm->SetRelativeRotation(InterpolatedRotation);
}

// Called to bind functionality to input
void ADefaultCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	const ASPlayerController* PlayerController = Cast<ASPlayerController>(GetController());

	//setup action bindings
	if (IsValid(Input) && IsValid(PlayerController))
	{
		if (const UPlayerInputActions* PlayerActions = Cast<UPlayerInputActions>(
			PlayerController->GetInputActionAsset()))
		{
			/** Default **/

			EPlayerInputActions::BindInput_TriggerOnly(Input, PlayerActions->Move, this, &ADefaultCameraPawn::Move);
			EPlayerInputActions::BindInput_TriggerOnly(Input, PlayerActions->Look, this, &ADefaultCameraPawn::Look);
			EPlayerInputActions::BindInput_TriggerOnly(Input, PlayerActions->Zoom, this, &ADefaultCameraPawn::Zoom);
			EPlayerInputActions::BindInput_TriggerOnly(Input, PlayerActions->Rotate, this, &ADefaultCameraPawn::Rotate);
			EPlayerInputActions::BindInput_StartTriggerComplete(Input, PlayerActions->Select, this,
			                                                    &ADefaultCameraPawn::Select,
			                                                    &ADefaultCameraPawn::SelectHold,
			                                                    &ADefaultCameraPawn::SelectEnd);
			EPlayerInputActions::BindInput_TriggerOnly(Input, PlayerActions->TestPlacement, this,
			                                           &ADefaultCameraPawn::TestPlacement);

			/** Placement **/
			EPlayerInputActions::BindInput_TriggerOnly(Input, PlayerActions->Place, this, &ADefaultCameraPawn::Place);
			EPlayerInputActions::BindInput_TriggerOnly(Input, PlayerActions->PlaceCancel, this,
			                                           &ADefaultCameraPawn::PlaceCancel);

			/** Build **/
			EPlayerInputActions::BindInput_TriggerOnly(Input, PlayerActions->BuildDeploy, this,
			                                           &ADefaultCameraPawn::BuildDeploy);
			EPlayerInputActions::BindInput_TriggerOnly(Input, PlayerActions->BuildCancel, this,
			                                           &ADefaultCameraPawn::BuildCancel);
		}
	}
}
