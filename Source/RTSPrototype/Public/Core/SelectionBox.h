// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPlayerController.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "SelectionBox.generated.h"

UCLASS()
class RTSPROTOTYPE_API ASelectionBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectionBox();

	UFUNCTION()
	void Start(FVector Position, const FRotator Rotation);
	UFUNCTION()
	void End();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FRotator StartRotation;
	UPROPERTY()
	TArray<AActor*> InBox;
	UPROPERTY()
	TArray<AActor*> CenterInBox;

	UFUNCTION()
	void Adjust() const;
	UFUNCTION()
	void ManageUnits();
	UFUNCTION()
	void HandleHighlight(AActor* ActorInBox, const bool Highlight = true) const;
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollider;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDecalComponent* DecalComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units Limit", meta = (AllowPrivateAccess = "true"))
	int UnitsLimit;

	int ConstantUnitsLimit;
	
	UPROPERTY()
	bool bBoxSelected;
	UPROPERTY()
	ASPlayerController* SPlayer;

};
