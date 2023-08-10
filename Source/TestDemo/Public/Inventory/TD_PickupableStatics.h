#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TD_PickupableStatics.generated.h"

class IPickupable;

/**
 * 
 */
UCLASS()
class UTD_PickupableStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UTD_PickupableStatics();

public:
	UFUNCTION(BlueprintPure)
	static TScriptInterface<IPickupable> GetFirstPickupableFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static void AddPickupToInventory(UTD_InventoryManagerComponent* InventoryComponent, TScriptInterface<IPickupable> Pickup);
};