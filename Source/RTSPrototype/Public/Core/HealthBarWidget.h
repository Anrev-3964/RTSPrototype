// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
	void SetBarPercent(float const value);

private:
	//BindWidgte : link a c++ widget in a created blueprint and manipulate it
	UPROPERTY(BlueprintReadOnly,  meta = (BindWidget, AllowPrivateAccess = true))
	UProgressBar* HealthBar = nullptr; //i will be se in the blueprint
};