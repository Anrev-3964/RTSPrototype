// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BTTask_Attack.h"

#include "AIController.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//check if target is in range
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		//UE_LOG(LogTemp, Warning, TEXT("il bersaglio e TROPPO DISTANTE per attacarlo"));
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);//segnala che l'azione richiede tempo
		return  EBTNodeResult::Succeeded;
	}

	//get Unit pawn and controller
	//FIRST CONST : the type of pointer is tryng to get cannot be modified
	//FIRST CONST : the pointer i cannot be modified
	auto const* const UnitController = OwnerComp.GetAIOwner();
	auto* const UnitPawn = Cast<ARTSPrototypeCharacter>(UnitController->GetPawn());
	
	//if Unit implements Icommandable interface, can execute the task
	if (auto* const Icommandable = Cast<ICommandable>(UnitPawn))
	{
		//dont attack if the unit is already attacking (anim montage in play)
		if (MontageHasFinished(UnitPawn))
		{
			//UE_LOG(LogTemp, Warning, TEXT("sono ABBASTANZA VICINO per attacarlo"));
			Icommandable->Attack();
		}
	}

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}


bool UBTTask_Attack::MontageHasFinished(ARTSPrototypeCharacter* const Unit)
{
	return Unit->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Unit->GetAttackMontage());
}
