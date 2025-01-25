// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_MoveToPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API UBTTask_MoveToPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	UBTTask_MoveToPatrolPoint();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UFUNCTION()
	void OnDestinationReached(FAIRequestID RequestID, EPathFollowingResult::Type MovementResult);

	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;

	UPROPERTY()
	float TimeToWait = 0.0f;
};
