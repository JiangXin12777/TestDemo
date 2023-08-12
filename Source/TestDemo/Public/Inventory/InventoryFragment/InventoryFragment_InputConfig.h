#pragma once

#include "Inventory/TD_InventoryItemFragment.h"
#include "InventoryFragment_InputConfig.generated.h"

class UNinjaInputSetupDataAsset;

UCLASS()
class UInventoryFragment_InputConfig : public UTD_InventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Input")
	TArray<TObjectPtr<UNinjaInputSetupDataAsset>> InputDataAsset;
};
