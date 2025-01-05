// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITriggerBoxArea.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UITriggerBoxArea : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTSPROTOTYPE_API IITriggerBoxArea
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool ActorIsATriggerArea() = 0;
};
