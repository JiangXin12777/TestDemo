#include "Inventory/TD_PickupableStatics.h"

#include "Inventory/TD_InventoryManagerComponent.h"
#include "Inventory/TD_InventoryPickup.h"
#include "Inventory/Interface/IPickupable.h"

UTD_PickupableStatics::UTD_PickupableStatics()
	: Super()
{
}

TScriptInterface<IPickupable> UTD_PickupableStatics::GetFirstPickupableFromActor(AActor* Actor)
{
	// If the actor is directly pickupable, return that.
	TScriptInterface<IPickupable> PickupableActor(Actor);
	if (PickupableActor)
	{
		return PickupableActor;
	}

	// If the actor isn't pickupable, it might have a component that has a pickupable interface.
	TArray<UActorComponent*> PickupableComponents = Actor ? Actor->GetComponentsByInterface(UPickupable::StaticClass()) : TArray<UActorComponent*>();
	if (PickupableComponents.Num() > 0)
	{
		// Get first pickupable, if the user needs more sophisticated pickup distinction, will need to be solved elsewhere.
		return TScriptInterface<IPickupable>(PickupableComponents[0]);
	}

	return TScriptInterface<IPickupable>();
}

void UTD_PickupableStatics::AddPickupToInventory(UTD_InventoryManagerComponent* InventoryComponent, TScriptInterface<IPickupable> Pickup)
{
	if (InventoryComponent && Pickup)
	{
		const FTD_InventoryPickup& PickupInventory = Pickup->GetPickupInventory();

		for (const FTD_PickupTemplate& Template : PickupInventory.Templates)
		{
			InventoryComponent->AddItemDefinition(Template.ItemDef, Template.StackCount);
		}

		// for (const FTD_PickupInstance& Instance : PickupInventory.Instances)
		// {
		// 	InventoryComponent->AddItemInstance(Instance.Item);
		// }
	}
}
