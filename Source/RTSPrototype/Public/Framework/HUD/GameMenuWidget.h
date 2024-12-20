// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenuWidget.generated.h"

class UButton;
class UBuildMenuWidget;
class USimpleUIButton;
/**
 * 
 */
UCLASS(Abstract)
class RTSPROTOTYPE_API UGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(BindWidget))
	UButton* Buildbutton;
	UPROPERTY(meta = (BindWidget))
	UBuildMenuWidget* BuildMenuWidget;

protected:
	UFUNCTION()
	void ToggleBuildMenu();
	UFUNCTION()
	void OnBuildItemsSelected();

	UPROPERTY()
	bool bBuildMenuOpen = false;
};
