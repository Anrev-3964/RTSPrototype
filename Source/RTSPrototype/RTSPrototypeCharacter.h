// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Selectable.h"
#include "GameFramework/Character.h"
#include "Commandable.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Core/FactionsEnum.h"
#include "Core/FactionsUtils.h"
#include "Animation/AnimMontage.h"
#include "Core/HealthBarWidget.h"
#include "RTSPrototypeCharacter.generated.h"

//Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTaken);

UCLASS(Blueprintable)
class ARTSPrototypeCharacter : public ACharacter, public ISelectable, public  ICommandable, public IFactionsUtils
{
	GENERATED_BODY()

public:
	ARTSPrototypeCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/**Statistics and Settings **/

	UPROPERTY(EditAnywhere,  Category = "Unit Perception")
	float UnitSightRadius;

	UPROPERTY(EditAnywhere,  Category = "Unit Perception")
	float UnitLoseSightRadius;

	UPROPERTY(EditAnywhere,  Category = "Unit Stats")
	float MovementSpeed;

	UPROPERTY(EditAnywhere,Category = "Unit Stats")
	float AttackValue;
	
	float MaxHealth{100.f};
	float Health;

public:

	/**ISelectable Interface**/
	virtual void Select() override;
	virtual void DeSelect() override;
	virtual void Highlight(const bool Highlight) override;
	/**End ISelectable Interface**/

	UFUNCTION(BlueprintCallable)
	FString GetFactionName() const;
	
	
	/**ICommandable Interface**/
	virtual  void MoveToDestination(const FVector Destination) override;
	virtual  void Attack() override;
	/**End ICommandable Interface**/

	/**IFactionUtils Interface**/
	virtual EFaction GetFaction()const override;
	/**End IFactionUtils Interface**/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Settings")
	EFaction CurrentFaction;
	/**End Statistics and Settings**/

	/** GetFunctions**/
	UBehaviorTree* GetBehaviorTree() const;
	UFUNCTION()
	float GetUnitSightRadius();
	float GetUnitLoseSightRadius();
	//float GetUnitAcceptanceRadius();
	UAnimMontage* GetAttackMontage() const;
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetHealth() const;
	/** GetFunctions**/

	/** Set Functions**/
	UFUNCTION(BlueprintCallable)
	void SetHealth(const float NewHealth);
	UFUNCTION(BlueprintCallable)
	void InflictDamage(const float Damage);
	/** Set Functions**/
	UPROPERTY()
	bool bSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montages")
	UAnimMontage* AttackMontage;
protected:
	//must be set in editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings",meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;
	//Delegate variable. it can be called by blueprint
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDamageTaken OnDamageTakenEvent;

	UFUNCTION()
	void DestroyCharacter();
	
};



