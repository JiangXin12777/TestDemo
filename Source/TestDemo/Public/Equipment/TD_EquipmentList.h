#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilitySet/TD_AbilitySetHandle.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "TD_EquipmentList.generated.h"

struct FTD_EquipmentList;
class UTD_EquipmentManagerComponent;
class UTD_EquipmentDefinition;
class UTD_EquipmentInstance;
class UTD_AbilitySystemComponent;

/**
 * 单件应用装备
 */
USTRUCT(BlueprintType)
struct FTD_AppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FTD_AppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FTD_EquipmentList;
	friend UTD_EquipmentManagerComponent;

	/** 装备定义 */
	UPROPERTY()
	TSubclassOf<UTD_EquipmentDefinition> EquipmentDefinition;

	/** 装备实例对象 */
	UPROPERTY()
	TObjectPtr<UTD_EquipmentInstance> Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FTD_AbilitySet_GrantedHandles GrantedHandles;
};

/**
 * 应用装备列表
 */
USTRUCT(BlueprintType)
struct FTD_EquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FTD_EquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FTD_EquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FTD_AppliedEquipmentEntry, FTD_EquipmentList>(Entries, DeltaParms, *this);
	}

	/** 通过给定的装备数据定义 添加装备 */
	UTD_EquipmentInstance* AddEntry(TSubclassOf<UTD_EquipmentDefinition> EquipmentDefinition);

	/** 通过装备实例 移除装备 */
	void RemoveEntry(UTD_EquipmentInstance* Instance);

private:
	UTD_AbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UTD_EquipmentManagerComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FTD_AppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FTD_EquipmentList> : public TStructOpsTypeTraitsBase2<FTD_EquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};