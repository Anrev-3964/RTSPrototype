// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetBlackBoardBoolValue.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API UBTTask_SetBlackBoardBoolValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_SetBlackBoardBoolValue();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "New Value")
	bool bNewBoolValue;
};
