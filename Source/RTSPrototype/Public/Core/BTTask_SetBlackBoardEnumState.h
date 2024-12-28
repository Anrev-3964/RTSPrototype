// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_SetBlackBoardEnumState.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API UBTTask_SetBlackBoardEnumState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_SetBlackBoardEnumState();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowedTypes = "Enum"))
	//FBlackboardKeySelector EnumKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Blackboard")
	uint8 DesiredEnumValue;
};
