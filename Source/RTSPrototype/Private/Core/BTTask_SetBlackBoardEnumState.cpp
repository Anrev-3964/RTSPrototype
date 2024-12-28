// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BTTask_SetBlackBoardEnumState.h"

UBTTask_SetBlackBoardEnumState::UBTTask_SetBlackBoardEnumState()
{
	NodeName = TEXT("Set Enum Value");
}

EBTNodeResult::Type UBTTask_SetBlackBoardEnumState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		/**
		if (EnumKey.SelectedKeyName != FName()) //FName : default value
		{
			BlackboardComp->SetValueAsEnum(EnumKey.SelectedKeyName, static_cast<uint8>(DesiredEnumValue));
			return EBTNodeResult::Succeeded;
		}
		**/
		{
			BlackboardComp->SetValueAsEnum(GetSelectedBlackboardKey(), static_cast<uint8>(DesiredEnumValue));
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
