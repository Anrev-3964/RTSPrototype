// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSPrototypeGameMode.h"
#include "RTSPrototypePlayerController.h"
#include "UObject/ConstructorHelpers.h"

ARTSPrototypeGameMode::ARTSPrototypeGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARTSPrototypePlayerController::StaticClass();
}