#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPickupable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UPickupable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IPickupable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual FTD_InventoryPickup GetPickupInventory() const = 0;
};
