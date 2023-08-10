#include "Equipment/TD_EquipmentList.h"

#include "Equipment/TD_EquipmentDefinition.h"
#include "Equipment/TD_EquipmentInstance.h"

FString FTD_AppliedEquipmentEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));
}

void FTD_EquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FTD_AppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
	}
}

void FTD_EquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FTD_AppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FTD_EquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	// 	for (int32 Index : ChangedIndices)
	// 	{
	// 		const FGameplayTagStack& Stack = Stacks[Index];
	// 		TagToCountMap[Stack.Tag] = Stack.StackCount;
	// 	}
}

UTD_EquipmentInstance* FTD_EquipmentList::AddEntry(TSubclassOf<UTD_EquipmentDefinition> EquipmentDefinition)
{
	UTD_EquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	
	const UTD_EquipmentDefinition* EquipmentCDO = GetDefault<UTD_EquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UTD_EquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UTD_EquipmentInstance::StaticClass();
	}
	
	FTD_AppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UTD_EquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;

	// if (UTD_AbilitySystemComponent* ASC = GetAbilitySystemComponent())
	// {
	// 	for (TObjectPtr<const UTD_AbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
	// 	{
	// 		AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
	// 	}
	// }
	// else
	// {
	// 	//@TODO: Warning logging?
	// }

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);


	MarkItemDirty(NewEntry);

	return Result;
}

void FTD_EquipmentList::RemoveEntry(UTD_EquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FTD_AppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			// if (UTD_AbilitySystemComponent* ASC = GetAbilitySystemComponent())
			// {
			// 	Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			// }

			Instance->DestroyEquipmentActors();
			

			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UTD_AbilitySystemComponent* FTD_EquipmentList::GetAbilitySystemComponent() const
{
	return nullptr;
}
