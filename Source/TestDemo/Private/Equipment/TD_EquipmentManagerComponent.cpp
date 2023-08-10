#include "Equipment/TD_EquipmentManagerComponent.h"

#include "Engine/ActorChannel.h"
#include "Equipment/TD_EquipmentDefinition.h"
#include "Equipment/TD_EquipmentInstance.h"
#include "Net/UnrealNetwork.h"

UTD_EquipmentManagerComponent::UTD_EquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

UTD_EquipmentInstance* UTD_EquipmentManagerComponent::EquipItem(TSubclassOf<UTD_EquipmentDefinition> EquipmentClass)
{
	UTD_EquipmentInstance* Result = nullptr;
	if (EquipmentClass != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentClass);
		if (Result != nullptr)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}

void UTD_EquipmentManagerComponent::UnequipItem(UTD_EquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

bool UTD_EquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FTD_AppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UTD_EquipmentInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UTD_EquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

void UTD_EquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UTD_EquipmentManagerComponent::UninitializeComponent()
{
	TArray<UTD_EquipmentInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator	
	for (const FTD_AppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UTD_EquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void UTD_EquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing LyraEquipmentInstances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FTD_AppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			UTD_EquipmentInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

UTD_EquipmentInstance* UTD_EquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UTD_EquipmentInstance> InstanceType)
{
	for (FTD_AppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UTD_EquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<UTD_EquipmentInstance*> UTD_EquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UTD_EquipmentInstance> InstanceType) const
{
	TArray<UTD_EquipmentInstance*> Results;
	for (const FTD_AppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UTD_EquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}