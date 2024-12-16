// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Selectable.h"
#include "GameFramework/Character.h"
#include "Commandable.h"
#include "Core/FactionsEnum.h"
#include "RTSPrototypeCharacter.generated.h"

UCLASS(Blueprintable)
class ARTSPrototypeCharacter : public ACharacter, public ISelectable,public  ICommandable
{
	GENERATED_BODY()

public:
	ARTSPrototypeCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	/**ISelectable Interface**/
	virtual void Select() override;
	virtual void DeSelect() override;
	virtual void Highlight(const bool Highlight) override;
	/**End ISelectable Interface**/

	UFUNCTION(BlueprintCallable)
	FString GetFactionName() const;
	
	/**ICommandable Interface**/
	virtual  void MoveToDestination(const FVector Destination) override;
	/**End ICommandable Interface**/

	/**Statistics and Settings **/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Settings")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Settings")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Settings")
	EFaction CurrentFaction;
	/**End Statistics and Settings**/

	UPROPERTY()
	bool bSelected;
};



