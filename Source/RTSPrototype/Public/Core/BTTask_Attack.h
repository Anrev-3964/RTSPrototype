// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RTSPrototype/RTSPrototypeCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API UBTTask_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_Attack();
	//execute istructions
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	//TRUE : animation montage ended
	bool MontageHasFinished(ARTSPrototypeCharacter* const Unit);
};
