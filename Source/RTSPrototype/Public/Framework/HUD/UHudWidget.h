// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/SPlayerController.h"
#include "UHudWidget.generated.h"

class UButton;
class UGameMenuWidget;
/**
 * 
 */
UCLASS(Abstract)
class RTSPROTOTYPE_API UUHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

protected:
	void CreateGameMenu();

	UPROPERTY()
	ASPlayerController* SPlayer;
	UPROPERTY()
	UWorld* WorldContext;

public:
	UPROPERTY()
	UGameMenuWidget* GameMenuWidget;
};
