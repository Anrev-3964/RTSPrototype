// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/HUD/UHudWidget.h"

#include "Framework/HUD/CustomHUD.h"
#include "Framework/HUD/GameMenuWidget.h"
#include "Kismet/GameplayStatics.h"

void UUHudWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	verify((WorldContext = GetWorld()) != nullptr);
	verify((SPlayer = Cast<ASPlayerController>(UGameplayStatics::GetPlayerController(WorldContext, 0))) != nullptr);

	CreateGameMenu();
}

void UUHudWidget::CreateGameMenu()
{
	if (!SPlayer || !WorldContext)
	{
		return;
	}

	if (const ACustomHUD* Hud = Cast<ACustomHUD>(SPlayer->GetHUD()))
	{
		if (Hud->GameMenuWidgetClass)
		{
			GameMenuWidget = Cast<UGameMenuWidget>(CreateWidget<UUserWidget>(WorldContext, Hud->GameMenuWidgetClass));
			if (GameMenuWidget)
			{
				GameMenuWidget->AddToViewport();
			}
		}
	}
}
