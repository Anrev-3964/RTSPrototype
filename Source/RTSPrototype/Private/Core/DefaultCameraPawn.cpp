// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DefaultCameraPawn.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "Core/SelectionBox.h"
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

	Camera =  CreateDefaultSubobject<UCameraComponent>("CameraComponent");
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
	CreateSelectionBox();
}

void ADefaultCameraPawn::MoveForward(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}

	FVector RightMovement = FVector(SpringArm->GetForwardVector().X, SpringArm->GetForwardVector().Y, 0.0f).GetSafeNormal() * AxisValue * MovementSpeed;
	TargetLocation += RightMovement;

	GetTerrainPosition(TargetLocation);
}

void ADefaultCameraPawn::MoveRight(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}

	FVector RightMovement = FVector(SpringArm->GetRightVector().X, SpringArm->GetRightVector().Y, 0.0f).GetSafeNormal() * AxisValue * MovementSpeed;
	TargetLocation += RightMovement;
	GetTerrainPosition(TargetLocation);
}

void ADefaultCameraPawn::Zoom(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}

	const float Zoom = AxisValue * 100.0f;
	TargetZoom = FMath::Clamp(TargetZoom + Zoom, ZoomMin, ZoomMax);
}

void ADefaultCameraPawn::RotateRight()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0, -45, 0));
}

void ADefaultCameraPawn::RotateLeft()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0, 45, 0));
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

void ADefaultCameraPawn::MouseLeftPressed()
{
	if (!SPlayer) return;
	SPlayer->HandleSelection(nullptr);
	bBoxSelected = false;
	LeftMouseHitLocation = SPlayer->GetMousePositionOnTerrain();
}

void ADefaultCameraPawn::MouseLeftInputHeld(float AxisValue)
{
	if (!SPlayer || AxisValue == 0.0f) return;
	if (SPlayer->GetInputKeyTimeDown(EKeys::LeftMouseButton) >= LeftMouseHoldThreshold)
	{
		if (!bBoxSelected && SelectionBox)
		{
			SelectionBox->Start(LeftMouseHitLocation, TargetRotation);
			bBoxSelected = true;
		}
	}
}

void ADefaultCameraPawn::MouseLeftReleased()
{
	if (SPlayer)
	{
			if (bBoxSelected && SelectionBox)
			{
				SelectionBox->End();
				bBoxSelected = false;
			}
			else
			{
				// Pass the selected actor to the player controller
				SPlayer->HandleSelection(GetSelectedObject());
			}
	}
}

void ADefaultCameraPawn::MouseRightPressed()
{
}

void ADefaultCameraPawn::MouseRightReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("ho il mouse destro"));
	if (!SPlayer) return;
	//Get Mouse posiiton on terrain
	RightMouseHitLocation = SPlayer->GetMousePositionOnTerrain();
	//Ask the player to move his troops
	SPlayer->MoveUnitsToDestination(RightMouseHitLocation);
	//TO DO : controllare cosa ha clicato il giocatore, sulla base di cosa ha cliclato, fare un azione
}

void ADefaultCameraPawn::CreateSelectionBox()
{
	if (!SelectionBoxClass) return;
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

void ADefaultCameraPawn::EdgeScroll()
{
	if (UWorld* WorldContext = GetWorld())
	{
		FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(WorldContext);
		const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(WorldContext);
		MousePosition = MousePosition * UWidgetLayoutLibrary::GetViewportScale(WorldContext);
		MousePosition.X = MousePosition.X/ViewportSize.X;
		MousePosition.Y = MousePosition.Y/ViewportSize.Y;

		//right/left
		if (MousePosition.X > 0.98f && MousePosition.X < 1.0f)
		{
			MoveRight(1.0f);
		}
		else if (MousePosition.X < 0.02f && MousePosition.X > 0.0f)
		{
			MoveRight(-1.0f);
		}

		//forward/backward
		if (MousePosition.Y > 0.98f && MousePosition.Y < 1.0f)
		{
			MoveForward(-1.0f);
		}
		else if (MousePosition.Y < 0.02f && MousePosition.Y > 0.0f)
		{
			MoveForward(1.0f);
		}
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


// Called every frame
void ADefaultCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	EdgeScroll();
	CameraBounds();

	const FVector InterpolatedLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, MovementSpeed);
	SetActorLocation(InterpolatedLocation);

	const float InterpolatedZoom = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, TargetZoom, DeltaTime, ZoomSpeed);
	SpringArm->TargetArmLength = InterpolatedZoom;

	const FRotator InterpolatedRotation = UKismetMathLibrary::RInterpTo(SpringArm->GetRelativeRotation(), TargetRotation, DeltaTime, RotationSpeed);
	SpringArm->SetRelativeRotation(InterpolatedRotation);

}

// Called to bind functionality to input
void ADefaultCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent -> BindAxis("Forward", this, &ADefaultCameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ADefaultCameraPawn::MoveRight);
	PlayerInputComponent->BindAxis("Zoom", this, &ADefaultCameraPawn::Zoom);
	PlayerInputComponent->BindAxis("RotateHorizontal", this, &ADefaultCameraPawn::RotateHorizontal);
	PlayerInputComponent->BindAxis("RotateVertical", this, &ADefaultCameraPawn::RotateVertical);
	PlayerInputComponent->BindAxis("MouseLeftAxis", this, &ADefaultCameraPawn::MouseLeftInputHeld);

	PlayerInputComponent->BindAction("RotateRight", IE_Pressed, this, &ADefaultCameraPawn::RotateRight);
	PlayerInputComponent->BindAction("RotateLeft", IE_Pressed, this, &ADefaultCameraPawn::RotateLeft);
	PlayerInputComponent->BindAction("Rotate", IE_Pressed, this, &ADefaultCameraPawn::EnableRotate);
	PlayerInputComponent->BindAction("Rotate", IE_Released, this, &ADefaultCameraPawn::DisableRotate);
	//leftKey
	PlayerInputComponent->BindAction("MouseLeft", IE_Pressed, this, &ADefaultCameraPawn::MouseLeftPressed);
	PlayerInputComponent->BindAction("MouseLeft", IE_Released, this, &ADefaultCameraPawn::MouseLeftReleased);
	//rightKey
	PlayerInputComponent->BindAction("MouseRight", IE_Pressed, this, &ADefaultCameraPawn::MouseRightPressed);
	PlayerInputComponent->BindAction("MouseRight", IE_Released, this, &ADefaultCameraPawn::MouseRightReleased);
}

