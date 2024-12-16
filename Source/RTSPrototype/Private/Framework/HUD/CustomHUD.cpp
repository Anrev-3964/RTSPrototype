// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/HUD/CustomHUD.h"

#include "Blueprint/UserWidget.h"

ACustomHUD::ACustomHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACustomHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ACustomHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomHUD::CreateHUD()
{
	if (HudClass)
	{
		/*HUD = CreateWidget<UHudWidget>(GetWorld(), HudClass);
		
		if (HUD != nullptr)
		{
			HUD->AddToViewport();
		}*/
	}
}
