// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Selectable.h"
#include "Framework/DataAssets/CharacterData.h"
#include "GameFramework/Character.h"
#include "RTSPrototypeCharacter.generated.h"

UCLASS(Blueprintable)
class ARTSPrototypeCharacter : public ACharacter, public ISelectable
{
	GENERATED_BODY()

public:
	ARTSPrototypeCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

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

protected:
	virtual void BeginPlay() override;
	/** Data Management **/
	void LoadData() const;
	UCharacterData* GetCharacterData() const;
	UMaterialInstance* GetHighlightMaterial() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta=(AllowedTypes="CharacterData"))
	FPrimaryAssetId CharacterDataAssetId;
	/** End Data Management**/
public:
	/**ISelectable Interface**/
	virtual void Select() override;
	virtual void DeSelect() override;
	virtual void Highlight(const bool Highlight) override;
	/**End ISelectable Interface**/

	UPROPERTY()
	bool bSelected;
};

