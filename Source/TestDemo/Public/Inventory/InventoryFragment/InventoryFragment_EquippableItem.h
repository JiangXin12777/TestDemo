#pragma once

#include "Inventory/TD_InventoryItemFragment.h"
#include "Templates/SubclassOf.h"
#include "InventoryFragment_EquippableItem.generated.h"

class UTD_EquipmentDefinition;
class UObject;

UCLASS()
class UInventoryFragment_EquippableItem : public UTD_InventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "TD|EquippableItem")
	TSubclassOf<UTD_EquipmentDefinition> EquipmentDefinition;
};
