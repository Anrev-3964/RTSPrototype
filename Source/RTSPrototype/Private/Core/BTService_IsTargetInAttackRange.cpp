// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BTService_IsTargetInAttackRange.h"
#include "Core/SAIController.h"
#include "RTSPrototype/RTSPrototypeCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTService_IsTargetInAttackRange::UBTService_IsTargetInAttackRange()
{
	//allow the service to work when it's reached by the behaivor tree
	bNotifyBecomeRelevant = true;
	//Set service name
	NodeName = TEXT("Is  Target In Attack Range");
}

void UBTService_IsTargetInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	//get Unit controller and Unit Pawn
	auto const* const UnitController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	auto const* const UnitPawn = Cast<ARTSPrototypeCharacter>(UnitController->GetPawn());
	
	//TO DO :confrotare la distanza tra l'unita e il bersaglio
	FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("TargetLocation"));;
	float DistanceToTarget = FVector::Dist(UnitPawn->GetActorLocation(),TargetLocation);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(),DistanceToTarget <= AttackRange);
}