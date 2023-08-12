#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "TD_InventoryList.generated.h"

class UTD_InventoryItemInstance;
class UActorComponent;
class UTD_InventoryManagerComponent;
class UTD_InventoryItemDefinition;

struct FTD_InventoryList;

/** 将商品添加到库存时显示的消息 */
USTRUCT(BlueprintType)
struct FTD_InventoryChangeMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UTD_InventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Delta = 0;
};

/** 库存中的单个物品 */
USTRUCT(BlueprintType)
struct FTD_InventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FTD_InventoryEntry()
	{}

	FString GetDebugString() const;

private:
	friend FTD_InventoryList;
	friend UTD_InventoryManagerComponent;

	/** 物品实例 */
	UPROPERTY()
	TObjectPtr<UTD_InventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

/** 库存物品清单 */
USTRUCT(BlueprintType)
struct FTD_InventoryList : public FFastArraySerializer
{
	GENERATED_BODY()
public:
	FTD_InventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FTD_InventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}
	
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	/** 网络数据同步 */
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FTD_InventoryEntry, FTD_InventoryList>(Entries, DeltaParms, *this);
	}

	/**
	 * 获取当前库存中的所有的物品实例对象
	 */
	TArray<UTD_InventoryItemInstance*> GetAllItems() const;

	/**
	 * 向库存列表中添加一个物品
	 */
	UTD_InventoryItemInstance* AddEntry(TSubclassOf<UTD_InventoryItemDefinition> ItemClass, int32 StackCount);

	/**
	 * 移除列表中的物品
	 */
	void RemoveEntry(UTD_InventoryItemInstance* Instance);

private:
	void BroadcastChangeMessage(FTD_InventoryEntry& Entry, int32 OldCount, int32 NewCount);

private:
	friend UTD_InventoryManagerComponent;

	/** 需要复制的物品列表 */
	UPROPERTY()
	TArray<FTD_InventoryEntry> Entries;

	/**  */
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FTD_InventoryList> : public TStructOpsTypeTraitsBase2<FTD_InventoryList>
{
	enum { WithNetDeltaSerializer = true };
};
