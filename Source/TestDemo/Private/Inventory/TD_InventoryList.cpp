#include "Inventory/TD_InventoryList.h"

#include "GameFramework/GameplayMessageSubsystem.h"
#include "NativeGameplayTags.h"
#include "GameplayTag/TD_GameplayTags.h"
#include "Inventory/TD_InventoryItemDefinition.h"
#include "Inventory/TD_InventoryItemFragment.h"
#include "Inventory/TD_InventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TD_InventoryList)

FString FTD_InventoryEntry::GetDebugString() const
{
	TSubclassOf<UTD_InventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
}

void FTD_InventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FTD_InventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FTD_InventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FTD_InventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FTD_InventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FTD_InventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

TArray<UTD_InventoryItemInstance*> FTD_InventoryList::GetAllItems() const
{
	TArray<UTD_InventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FTD_InventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr)
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

UTD_InventoryItemInstance* FTD_InventoryList::AddEntry(TSubclassOf<UTD_InventoryItemDefinition> ItemDef, int32 StackCount)
{
	UTD_InventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FTD_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UTD_InventoryItemInstance>(OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);
	for (UTD_InventoryItemFragment* Fragment : GetDefault<UTD_InventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;

	//const ULyraInventoryItemDefinition* ItemCDO = GetDefault<ULyraInventoryItemDefinition>(ItemDef);
	MarkItemDirty(NewEntry);

	return Result;
}

void FTD_InventoryList::AddEntry(UTD_InventoryItemInstance* Instance)
{
	unimplemented();
}

void FTD_InventoryList::RemoveEntry(UTD_InventoryItemInstance* Instance)
{
	//unimplemented();
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FTD_InventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

void FTD_InventoryList::BroadcastChangeMessage(FTD_InventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	FTD_InventoryChangeMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	MessageSystem.BroadcastMessage(TD::Inventory_Message_StackChanged, Message);
}
