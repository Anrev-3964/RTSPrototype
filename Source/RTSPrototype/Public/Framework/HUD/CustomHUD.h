// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Framework/HUD/UHudWidget.h"
#include "CustomHUD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RTSPROTOTYPE_API ACustomHUD : public AHUD
{
	GENERATED_BODY()

	public:
	ACustomHUD();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void CreateHUD();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> HudClass;

	UPROPERTY()
	UUHudWidget* HUD;
	
	virtual void BeginPlay() override;
	
};
