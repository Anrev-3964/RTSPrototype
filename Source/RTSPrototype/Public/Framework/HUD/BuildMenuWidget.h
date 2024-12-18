// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildMenuWidget.generated.h"

class UBuildComponent;
class UListView;
class UBorder;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildItemsSelectedDelegate);

/**
 * 
 */
UCLASS(Abstract)
class RTSPROTOTYPE_API UBuildMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* MenuBorder;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UListView* BuildItemsList;

	UFUNCTION()
	void DisplayBuildMenu();
	UFUNCTION()
	void EndDisplayBuildMenu() const;

	FOnBuildItemsSelectedDelegate OnBuildItemsSelectedEvent;

protected:
	UFUNCTION()
	void OnBuildModeEnteredEvent();

	UPROPERTY()
	APlayerController* SPlayer;
	UPROPERTY()
	UBuildComponent* BuildComponent;
	UPROPERTY()
	UAssetManager* AssetManager;
};
