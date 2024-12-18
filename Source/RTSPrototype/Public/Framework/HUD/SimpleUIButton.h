// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimpleUIButton.generated.h"

class UTextBlock;
class UButton;
class UImage;
/**
 * 
 */


UCLASS()
class RTSPROTOTYPE_API USimpleUIButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ButtonTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* Button;
	/*UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ButtonImage;*/


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FString ButtonText;
};
