#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Commandable.generated.h"

UINTERFACE(MinimalAPI)
class UCommandable : public  UInterface
{
	GENERATED_BODY()
};

class RTSPROTOTYPE_API ICommandable
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual void MoveToDestination(const FVector Destination) = 0;
};
