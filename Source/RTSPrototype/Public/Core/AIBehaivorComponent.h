// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIBehaivorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTSPROTOTYPE_API UAIBehaivorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIBehaivorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Data")
	TMap<AActor*, float> PatrolPoints;

	UFUNCTION(BlueprintCallable)
	int GetPatrolPointsMapLenght();

	UFUNCTION(BlueprintCallable)
	const AActor* GetPatrolPointActorByIndex(int Index);
	UFUNCTION(BlueprintCallable)
	float GetPatrolPointWaitTimeByIndex(int Index);
};
