// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FactionsEnum.h"
#include "FactionsUtils.generated.h"

UINTERFACE(MinimalAPI)
//make the Interface visible to unreal reflection system 
class UFactionsUtils : public  UInterface 
{
	GENERATED_BODY()
};

class RTSPROTOTYPE_API IFactionsUtils
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual EFaction GetFaction()const = 0 ;
};
