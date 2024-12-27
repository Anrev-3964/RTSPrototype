// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsTargetInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API UBTService_IsTargetInAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public :
	//default constructor
	UBTService_IsTargetInAttackRange();
	//the equivalent of th ebehaivor tree
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
