// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BTTask_MoveToPatrolPoint.h"

#include "AIController.h"
#include "Core/SAIController.h"
#include "RTSPrototype/RTSPrototypeCharacter.h"

UBTTask_MoveToPatrolPoint::UBTTask_MoveToPatrolPoint()
{
	NodeName = TEXT("PatrolBetweenPoints");
}

EBTNodeResult::Type UBTTask_MoveToPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	//Ottenere l'unita
	auto UnitController = OwnerComp.GetAIOwner();
	auto* const UnitPawn = Cast<ARTSPrototypeCharacter>(UnitController->GetPawn());

	if (!UnitPawn || !UnitController) return EBTNodeResult::Failed;

	//Cerca il component che tiene l'array map di patrolPoints
	if (UAIBehaivorComponent* Component = UnitPawn->FindComponentByClass<UAIBehaivorComponent>())
	{
		//prende l'indice dal Controller
		if (ASAIController* Controller = Cast<ASAIController>(UnitController))
		{
			 int DesiredIndex = Controller->GetCurrentPatrolIndex();
			//Ottenere la posizione desiderata
			 if (const AActor* PatrolPointActor = Component->GetPatrolPointActorByIndex(DesiredIndex))
			 {
			 	FVector DesiredLocation = PatrolPointActor->GetActorLocation();

			 	//Ordinare al pawn di muoversi
			 	Controller->MoveToLocation(DesiredLocation);
			 	TimeToWait = Component->GetPatrolPointWaitTimeByIndex(DesiredIndex);
			 	Controller->IncrementPatrolIndex();
			 	Controller->ReceiveMoveCompleted.AddDynamic(this, &UBTTask_MoveToPatrolPoint::OnDestinationReached);
			 	return EBTNodeResult::InProgress;
			 }
		}
		
	}
	return EBTNodeResult::Failed;
	
}

void UBTTask_MoveToPatrolPoint::OnDestinationReached(FAIRequestID RequestID, EPathFollowingResult::Type MovementResult)
{
	auto* Controller = CachedOwnerComp->GetAIOwner();
	if (Controller)
	{
		Controller->ReceiveMoveCompleted.RemoveDynamic(this, &UBTTask_MoveToPatrolPoint::OnDestinationReached);
	}
	if (MovementResult == EPathFollowingResult::Success)
	{
		// Se il movimento ha avuto successo, segnala la fine della task come riuscita

		if (CachedOwnerComp)
		{
			FTimerHandle TimerHandle;
			CachedOwnerComp->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
				{
				FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
				},
				TimeToWait, //tempo di attesa
				false);//false (il timer non viene ripetuto)
		}
	}
	else
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Failed);
	}

	//TO DO : potrei aggiungere della logica extra se EPathFollowingResult::Abort
}



