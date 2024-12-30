// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectionBox.h"
#include "InputActionValue.h"
#include "SPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "DefaultCameraPawn.generated.h"



UCLASS()
class RTSPROTOTYPE_API ADefaultCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADefaultCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void EnableRotate();
	UFUNCTION()
	void DisableRotate();
	UFUNCTION()
	void EdgeScroll();
	UFUNCTION()
	void GetTerrainPosition(FVector& TerrainPosition) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float MovementSpeed = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float RotationSpeed = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float RotationPitchMin = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float RotationPitchMax = 80.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float ZoomSpeed = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float ZoomMin = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float ZoomMax = 3000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float EdgeScrollSpeed = 20.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UFUNCTION()
	void CameraBounds();
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USceneComponent* SceneComponent;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = true))
	UCameraComponent* Camera;
	UPROPERTY()
	FVector TargetLocation;
	UPROPERTY()
	FRotator TargetRotation;
	UPROPERTY()
	float TargetZoom;
	UPROPERTY()
	bool bCanRotate;

	//Mouse Input
protected:

	UFUNCTION()
	AActor* GetSelectedObject();
	UFUNCTION()
	void MouseRightPressed();
	//UFUNCTION()
	//void MouseRightReleased();
	UFUNCTION()
	void CreateSelectionBox();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse Settings")
	float LeftMouseHoldThreshold = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouse Settings")
	TSubclassOf<ASelectionBox> SelectionBoxClass;

	/**Mouse Input **/
	UPROPERTY()
	ASPlayerController* SPlayer;
	UPROPERTY()
	ASelectionBox* SelectionBox;
	UPROPERTY()
	bool bBoxSelected;
	UPROPERTY()
	FVector SelectHitLocation;
	UPROPERTY()
	UBuildComponent* BuildComponent;

	/** Enahnced Input **/
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Rotate(const FInputActionValue& Value);
	UFUNCTION()
	void Zoom(const FInputActionValue& Value);
	UFUNCTION()
	void Select(const FInputActionValue& Value);
	UFUNCTION()
	void HandlePlayerRigthClick(const FInputActionValue& Value);
	UFUNCTION()
	void SelectHold(const FInputActionValue& Value);
	UFUNCTION()
	void SelectEnd(const FInputActionValue& Value);
	UFUNCTION()
	void TestPlacement(const FInputActionValue& Value);

	/** Placement **/

	UFUNCTION()
	void Place(const FInputActionValue& Value);
	UFUNCTION()
	void PlaceCancel(const FInputActionValue& Value);

	/** Build **/

	UFUNCTION()
	void BuildDeploy(const FInputActionValue& Value);
	UFUNCTION()
	void BuildCancel(const FInputActionValue& Value);
	
	UPROPERTY()
	FVector LeftMouseHitLocation;
	UPROPERTY()
	FVector RightMouseHitLocation;

	/**Units Managment**/
	UFUNCTION()
	void PawnMove();
};
