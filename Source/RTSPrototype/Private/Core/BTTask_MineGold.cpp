// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BTTask_MineGold.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Buildings/GoldMine.h"
#include "RTSPrototype/RTSPrototypeCharacter.h"


UBTTask_MineGold::UBTTask_MineGold()
{
	NodeName = TEXT("MineGold");
}

EBTNodeResult::Type UBTTask_MineGold::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const* const UnitController = OwnerComp.GetAIOwner();
	auto* const UnitPawn = Cast<ARTSPrototypeCharacter>(UnitController->GetPawn());

	if (!UnitPawn || !UnitController)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, " UBTTask_MineGold : qualcosa e andato storto");
		}
		return  EBTNodeResult::Failed;
	}
	
	//if Unit implements Icommandable interface, can execute the task
	if (auto* const Icommandable = Cast<ICommandable>(UnitPawn))
	{
		//dont Mine if the unit is already attacking (anim montage in play)
		if (MontageHasFinished(UnitPawn))
		{
			UObject* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey());
			if (AGoldMine* GoldMine = Cast<AGoldMine>(Target))
			{
				Icommandable->EstractGoldFromMine(GoldMine);
				FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
		return EBTNodeResult::Failed;
	
}

bool UBTTask_MineGold::MontageHasFinished(ARTSPrototypeCharacter* const Unit)
{
	return Unit->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Unit->GetMiningMontage());
}


