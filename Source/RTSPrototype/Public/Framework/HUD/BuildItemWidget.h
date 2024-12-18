// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleUIButton.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Core/SPlayerController.h"
#include "BuildItemWidget.generated.h"


class UBuildItemDataAsset;
class UBuildComponent;
/**
 * 
 */
UCLASS(Abstract)
class RTSPROTOTYPE_API UBuildItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	UPROPERTY(meta = (BindWidget))
	USimpleUIButton* PlaceBuildButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleText;
	UPROPERTY(meta = (BindWidget))
	UImage* Image;

protected:
	UFUNCTION()
	void OnPlaceBuildItemSelected();

	/** Begin IUserObjectListEntry Interface **/
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	/** End IUserObjectListEntry Interface **/

	UPROPERTY()
	ASPlayerController* SPlayer;
	UPROPERTY()
	UBuildItemDataAsset* Data;
	UPROPERTY()
	UBuildComponent* BuildComponent;
};
