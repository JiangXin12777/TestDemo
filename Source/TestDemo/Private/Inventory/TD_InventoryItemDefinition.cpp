#include "Inventory/TD_InventoryItemDefinition.h"

#include "Inventory/TD_InventoryItemFragment.h"

UTD_InventoryItemDefinition::UTD_InventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UTD_InventoryItemFragment* UTD_InventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<UTD_InventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UTD_InventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

const UTD_InventoryItemFragment* UTD_InventoryFunctionLibrary::FindItemDefinitionFragment(
	TSubclassOf<UTD_InventoryItemDefinition> ItemDef, TSubclassOf<UTD_InventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UTD_InventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
