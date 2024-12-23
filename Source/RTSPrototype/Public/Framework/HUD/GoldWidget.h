// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoldWidget.generated.h"

class UTextBlock;
class ARTSPlayerState;
class UImage;
/**
 * 
 */
UCLASS()
class RTSPROTOTYPE_API UGoldWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* GoldImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* GoldText;
	UPROPERTY()
	ARTSPlayerState* PlayerState;

protected:
	UFUNCTION()
	void SetGoldText();
};
