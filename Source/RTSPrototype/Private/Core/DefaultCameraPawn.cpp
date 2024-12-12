// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DefaultCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
	
}

void ADefaultCameraPawn::MoveForward(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}

	FVector ForwardMovement = SpringArm->GetForwardVector() * AxisValue * MovementSpeed;
	TargetLocation += ForwardMovement;
}

void ADefaultCameraPawn::MoveRight(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}

	FVector RightMovement = SpringArm->GetRightVector() * AxisValue * MovementSpeed;
	TargetLocation += RightMovement;
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
	TargetLocation = FVector(TargetLocation.X, TargetLocation.Y, 0);
}


// Called every frame
void ADefaultCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	PlayerInputComponent->BindAction("RotateRight", IE_Pressed, this, &ADefaultCameraPawn::RotateRight);
	PlayerInputComponent->BindAction("RotateLeft", IE_Pressed, this, &ADefaultCameraPawn::RotateLeft);
	PlayerInputComponent->BindAction("Rotate", IE_Pressed, this, &ADefaultCameraPawn::EnableRotate);
	PlayerInputComponent->BindAction("Rotate", IE_Released, this, &ADefaultCameraPawn::DisableRotate);

}

