#pragma once

#include "CoreMinimal.h"
#include "TD_InventoryItemFragment.generated.h"

class UTD_InventoryItemInstance;

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class UTD_InventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:
	UTD_InventoryItemFragment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnInstanceCreated(UTD_InventoryItemInstance* Instance) const {}
};