// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RTSPrototype/RTSPrototypeCharacter.h"
#include "BTTask_MineGold.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API UBTTask_MineGold : public UBTTask_BlackboardBase
{
	UBTTask_MineGold();
	GENERATED_BODY()
	public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//TRUE : animation montage ended
	bool MontageHasFinished(ARTSPrototypeCharacter* const Unit);
};
