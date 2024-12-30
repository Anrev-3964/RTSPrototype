#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Commandable.generated.h"

UINTERFACE(MinimalAPI)
//make the Interface visible to unreal reflection system 
class UCommandable : public  UInterface 
{
	GENERATED_BODY()
};

class RTSPROTOTYPE_API ICommandable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void MoveToDestination(const FVector Destination) = 0;
	
	UFUNCTION()
	virtual void Attack() = 0;
	UFUNCTION()
	virtual void ChaseTarget(AActor* Target) = 0; 
};
