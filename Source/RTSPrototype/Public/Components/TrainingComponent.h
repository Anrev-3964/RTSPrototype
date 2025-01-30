// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RTSPrototype/RTSPrototypeCharacter.h"
#include "TrainingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTSPROTOTYPE_API UTrainingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTrainingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanTrainUnits = true;
protected:
	/**Trining Settings**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Training Settings")
	TArray<TSubclassOf<ARTSPrototypeCharacter>> TrainableUnits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Training Settings")
	int MaxUnits = {1};
	UPROPERTY(BlueprintReadWrite,Category = "Training Settings")
	FVector UnitSpawnLocation = {FVector::ZeroVector};
	UPROPERTY(BlueprintReadWrite,Category ="Training Settings")
	FRotator  UnitSpawnRotation = {	FRotator::ZeroRotator};

	/**  **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units Data")
	TArray<ARTSPrototypeCharacter*> TrainedUnits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTraining = false;
	UFUNCTION()
	void RemoveDeadUnits();
	void TrainNewUnits();
	void SpawnUnit(TSubclassOf<ARTSPrototypeCharacter> UnitToSpawn);

private:
	FTimerHandle TrainingTimerHandle;
};
