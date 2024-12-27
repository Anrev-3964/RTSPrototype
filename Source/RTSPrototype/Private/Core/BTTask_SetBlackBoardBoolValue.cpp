// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BTTask_SetBlackBoardBoolValue.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetBlackBoardBoolValue::UBTTask_SetBlackBoardBoolValue(): bNewBoolValue(false)
{
	NodeName = TEXT("Set Bool Value");
}

EBTNodeResult::Type UBTTask_SetBlackBoardBoolValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsBool(GetSelectedBlackboardKey(),bNewBoolValue);
		UE_LOG(LogTemp, Warning, TEXT("valore della nuova blackboard %d"), BlackboardComp->GetValueAsBool(GetSelectedBlackboardKey()));
	}

	return EBTNodeResult::Succeeded;
}
