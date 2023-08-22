#include "Equipment/TD_QuickBarComponent.h"

#include "Equipment/TD_EquipmentDefinition.h"
#include "Equipment/TD_EquipmentInstance.h"
#include "Equipment/TD_EquipmentManagerComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTag/TD_GameplayTags.h"
#include "Inventory/TD_InventoryItemInstance.h"
#include "Inventory/InventoryFragment/InventoryFragment_EquippableItem.h"
#include "Net/UnrealNetwork.h"

UTD_QuickBarComponent::UTD_QuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UTD_QuickBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Slots);
	DOREPLIFETIME(ThisClass, ActiveSlotIndex);
}

void UTD_QuickBarComponent::BeginPlay()
{
	Super::BeginPlay();

	// 初始化道具插槽
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
}

void UTD_QuickBarComponent::CycleActiveSlotForward()
{
	if (Slots.Num() < 2)
	{
		return;
	}

	const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num()-1 : ActiveSlotIndex);
	int32 NewIndex = ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex + 1) % Slots.Num();
		if (Slots[NewIndex] != nullptr)
		{
			SetActiveSlotIndex(NewIndex);
			return;
		}
	} while (NewIndex != OldIndex);
}

void UTD_QuickBarComponent::CycleActiveSlotBackward()
{
	if (Slots.Num() < 2)
	{
		return;
	}

	const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num()-1 : ActiveSlotIndex);
	int32 NewIndex = ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex - 1 + Slots.Num()) % Slots.Num();
		if (Slots[NewIndex] != nullptr)
		{
			SetActiveSlotIndex(NewIndex);
			return;
		}
	} while (NewIndex != OldIndex);
}

void UTD_QuickBarComponent::SetActiveSlotItem(UTD_InventoryItemInstance* InItem)
{
	if (!InItem || !Slots.Contains(InItem))
		return;

	const int32 TempIndex =  Slots.IndexOfByKey(InItem);
	if (TempIndex != INDEX_NONE)
	{
		SetActiveSlotIndex(TempIndex);
	}
}

const TArray<UTD_InventoryItemInstance*>& UTD_QuickBarComponent::GetSlots() const
{
	return Slots;
}

UTD_InventoryItemInstance* UTD_QuickBarComponent::GetActiveSlotItem() const
{
	return Slots.IsValidIndex(ActiveSlotIndex) ? Slots[ActiveSlotIndex] : nullptr;
}

UTD_EquipmentInstance* UTD_QuickBarComponent::GetActiveEquippedItem() const
{
	return EquippedItem;
}

int32 UTD_QuickBarComponent::GetNextFreeItemSlot() const
{
	int32 SlotIndex = 0;
	for (TObjectPtr<UTD_InventoryItemInstance> ItemPtr : Slots)
	{
		if (ItemPtr == nullptr)
		{
			return SlotIndex;
		}
		++SlotIndex;
	}

	return INDEX_NONE;
}

void UTD_QuickBarComponent::AddItemToSlot(int32 SlotIndex, UTD_InventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
			OnRep_Slots();
		}
	}
}

UTD_InventoryItemInstance* UTD_QuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	UTD_InventoryItemInstance* Result = nullptr;

	if (ActiveSlotIndex == SlotIndex)
	{
		UnequipItemInSlot();
		ActiveSlotIndex = -1;
	}

	if (Slots.IsValidIndex(SlotIndex))
	{
		Result = Slots[SlotIndex];

		if (Result != nullptr)
		{
			Slots[SlotIndex] = nullptr;
			OnRep_Slots();
		}
	}

	return Result;
}

void UTD_QuickBarComponent::UnequipItemInSlot()
{
	if (UTD_EquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem != nullptr)
		{
			EquipmentManager->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}

void UTD_QuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	// 拿到库存物品实例
	if (UTD_InventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		// 获取其可装备能力配置，如果为空则其不可以作为玩家的手持道具
		if (const UInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UInventoryFragment_EquippableItem>())
		{
			TSubclassOf<UTD_EquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef != nullptr)
			{
				if (UTD_EquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem != nullptr)
					{
						EquippedItem->SetInstigator(SlotItem);
					}
				}
			}
		}
	}
}

UTD_EquipmentManagerComponent* UTD_QuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<UTD_EquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void UTD_QuickBarComponent::OnRep_Slots()
{
	FTD_QuickBarSlotsChangedMessage Message;
	Message.Owner = GetOwner();
	Message.Slots = Slots;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(TD::QuickBar_Message_SlotsChanged, Message);
}

void UTD_QuickBarComponent::OnRep_ActiveSlotIndex()
{
	FTD_QuickBarActiveIndexChangedMessage Message;
	Message.Owner = GetOwner();
	Message.ActiveIndex = ActiveSlotIndex;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(TD::QuickBar_Message_ActiveIndexChanged, Message);
}

void UTD_QuickBarComponent::SetActiveSlotIndex_Implementation(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();

		ActiveSlotIndex = NewIndex;

		EquipItemInSlot();

		OnRep_ActiveSlotIndex();
	}
}
