#include "Inventory/TD_InventoryManagerComponent.h"

#include "Engine/ActorChannel.h"
#include "Inventory/TD_InventoryItemDefinition.h"
#include "Inventory/TD_InventoryItemInstance.h"
#include "Net/UnrealNetwork.h"

class FLifetimeProperty;
struct FReplicationFlags;

UTD_InventoryManagerComponent::UTD_InventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

bool UTD_InventoryManagerComponent::CanAddItemDefinition(TSubclassOf<UTD_InventoryItemDefinition> ItemDef, int32 StackCount)
{
	//@TODO: Add support for stack limit / uniqueness checks / etc...
	return true;
}

UTD_InventoryItemInstance* UTD_InventoryManagerComponent::AddItemDefinition(TSubclassOf<UTD_InventoryItemDefinition> ItemDef, int32 StackCount)
{
	UTD_InventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;	
}

void UTD_InventoryManagerComponent::RemoveItemInstance(UTD_InventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UTD_InventoryItemInstance*> UTD_InventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UTD_InventoryItemInstance* UTD_InventoryManagerComponent::FindFirstItemStackByDefinition(TSubclassOf<UTD_InventoryItemDefinition> ItemDef) const
{
	for (const FTD_InventoryEntry& Entry : InventoryList.Entries)
	{
		UTD_InventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

int32 UTD_InventoryManagerComponent::GetTotalItemCountByDefinition(TSubclassOf<UTD_InventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FTD_InventoryEntry& Entry : InventoryList.Entries)
	{
		UTD_InventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				++TotalCount;
			}
		}
	}

	return TotalCount;
}

bool UTD_InventoryManagerComponent::ConsumeItemsByDefinition(TSubclassOf<UTD_InventoryItemDefinition> ItemDef, int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (UTD_InventoryItemInstance* Instance = FindFirstItemStackByDefinition(ItemDef))
		{
			InventoryList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

bool UTD_InventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FTD_InventoryEntry& Entry : InventoryList.Entries)
	{
		UTD_InventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UTD_InventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing ULyraInventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FTD_InventoryEntry& Entry : InventoryList.Entries)
		{
			UTD_InventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

void UTD_InventoryManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}