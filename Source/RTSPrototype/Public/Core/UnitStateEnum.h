// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitStateEnum.generated.h"

/**
 * 
 */
UENUM(BlueprintType) // Aggiungi questa macro per esporre l'enum ai Blueprint
enum class EUnitState : uint8
{
    WaitingForOrders UMETA(DisplayName = "WaitingForOrders"),    // Quando l'unità non ha nessun ordine dal player -> logica autonoma
    MovingToDestination UMETA(DisplayName = "MovingToDestination"), // Quando l'unità ha ricevuto l'ordine dal player di muoversi
    AttackingTarget UMETA(DisplayName = "AttackingTarget"),     // Quando l'unità ha ricevuto un bersaglio dal player -> logica di inseguimento e attacco
    MiningGold UMETA(DisplayName = "MiningGold")                // Quando l'unità ha ricevuto ordine dal player di raccogliere oro -> logica di raccolta dell'oro
};
