// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Selectable.h"
#include "Framework/DataAssets/CharacterData.h"
#include "GameFramework/Character.h"
#include "Commandable.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Core/FactionsEnum.h"
#include "Core/FactionsUtils.h"
#include "Animation/AnimMontage.h"
#include "Core/AIBehaivorComponent.h"
#include "Core/UnitData.h"
#include "RTSPrototypeCharacter.generated.h"

class UTexture;
//Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitSelected,bool, UnitSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrderChanged,EUnitState, NewOrder);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldEstracted,int, GoldEstracted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPeonSelected,FVector, PeonPosition);

UCLASS(Blueprintable)
class ARTSPrototypeCharacter : public ACharacter, public ISelectable, public ICommandable, public IFactionsUtils
{
	GENERATED_BODY()

public:
	ARTSPrototypeCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UFUNCTION()
	void PlayRandomSound();


protected:
	/**Actor Compoennts **/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAIBehaivorComponent* ActorAIBehaivorComponent;
	virtual void BeginPlay() override;
	/** Data Management **/
	void LoadData() const;
	UCharacterData* GetCharacterData() const;
	UMaterialInstance* GetHighlightMaterial() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta=(AllowedTypes="CharacterData"))
	FPrimaryAssetId CharacterDataAssetId;
	/** End Data Management**/

	/**Statistics and Settings **/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AttackValue;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SpawnDelay;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture* UnitImage;
	UPROPERTY(BlueprintReadWrite,  meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	UPROPERTY(BlueprintReadOnly,  meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MiningMontage;
	//Unit Data Asset
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Unit Stats")
	UUnitData* UnitData;

	bool bCanMine = false;

	UFUNCTION()
	void AssignUnitStatsFromDataAsset();
	UFUNCTION()
	void ManageBuildMenu(bool bIsSelected);

	UFUNCTION()
	void PlayAudioClip(USoundBase* ClipToPlay);

public:
	/**ISelectable Interface**/
	virtual void Select() override;
	virtual void DeSelect() override;
	virtual void Highlight(const bool Highlight) override;
	virtual void AttackSelectable(const float DamageAmount) override;
	/**End ISelectable Interface**/

	UFUNCTION(BlueprintCallable)
	FString GetFactionName() const;


	/**ICommandable Interface**/
	virtual  void MoveToDestination(const FVector Destination) override;
	virtual  void Attack() override;
	virtual  void ChaseTarget(AActor* Target) override;
	virtual void  StartMiningGold(AActor* Target) override;
	virtual void EstractGoldFromMine(AActor* Target) override;
	/**End ICommandable Interface**/

	/**IFactionUtils Interface**/
	virtual EFaction GetFaction()const override;
	virtual void SetCurrentFaction(EFaction NewFaction) override;
	/**End IFactionUtils Interface**/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Settings")
	EFaction CurrentFaction;
	/**End Statistics and Settings**/

	/** GetFunctions**/
	UBehaviorTree* GetBehaviorTree() const;

	UAnimMontage* GetAttackMontage() const;
	UAnimMontage* GetMiningMontage() const;

	UFUNCTION(BlueprintCallable)

	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	UUnitData* GetUnitData() const;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetSpawnDelay() const;
	UFUNCTION(BlueprintCallable)
	bool HasToPatrol() const;
	/** GetFunctions**/

	/** Set Functions**/
	UFUNCTION(BlueprintCallable)
	void SetHealth(const float NewHealth);
	UFUNCTION(BlueprintCallable)
	void InflictDamage(const float Damage);
	/** Set Functions**/

	UPROPERTY()
	bool bSelected;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDamageTaken OnDamageTakenEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttack OnAttack;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeath OnDeath;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUnitSelected OnUnitSelected;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnOrderChanged OnOrderChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGoldEstracted OnGoldEstracted;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPeonSelected OnPeonSelected;

protected:
	//must be set in editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;
	//Delegate variable. it can be called by blueprint

	/**
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDamageTaken OnDamageTakenEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttack OnAttack;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeath OnDeath;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUnitSelected OnUnitSelected;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnOrderChanged OnOrderChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGoldEstracted OnGoldEstracted;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPeonSelected OnPeonSelected;
	**/
	UFUNCTION()
	void DestroyCharacter();
};
