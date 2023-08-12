#pragma once

#include "Inventory/TD_InventoryItemFragment.h"
#include "InventoryFragment_HitWidgetConfig.generated.h"

class UUserWidget;

UCLASS()
class UInventoryFragment_HitWidgetConfig : public UTD_InventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Hit")
	TArray<TSubclassOf<UUserWidget>> HitWidgets;
};
