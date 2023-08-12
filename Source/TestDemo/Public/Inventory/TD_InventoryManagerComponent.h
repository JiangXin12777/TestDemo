#pragma once

#include "CoreMinimal.h"
#include "TD_InventoryList.h"
#include "TD_InventoryManagerComponent.generated.h"

class UTD_InventoryItemDefinition;
class UTD_InventoryItemInstance;

/**
 * Manages an inventory
 */
UCLASS(BlueprintType)
class UTD_InventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UTD_InventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	bool CanAddItemDefinition(TSubclassOf<UTD_InventoryItemDefinition> ItemDef, int32 StackCount = 1);

	/**
	 * 通过给定的库存物品定义向当前仓库添加库存实例
	 * @param ItemDef 库存物的相关定义（要生成的拾取 Actor、授予的能力以及要添加的标签）
	 * @param StackCount ~~
	 *
	 * @return 将添加成功后的对象实例返回
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UTD_InventoryItemInstance* AddItemDefinition(TSubclassOf<UTD_InventoryItemDefinition> ItemDef, int32 StackCount = 1);

	/**
	 * 从仓库中移除当前实例对象
	 * @param ItemInstance 库存物品实例对象
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveItemInstance(UTD_InventoryItemInstance* ItemInstance);

	/**
	 * 获取当前仓库中所有的库存物品实例
	 *
	 * @return
	 */
	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	TArray<UTD_InventoryItemInstance*> GetAllItems() const;

	/**
	 * 通过库存物品定义查找当前库存中是否存在该物品实例
	 * @param ItemDef 库存物品相关定义
	 *
	 * @return 当前仓库中存在的该实例对象
	 */
	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UTD_InventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UTD_InventoryItemDefinition> ItemDef) const;

	/**
	 * 通过库存物定义获取当前物品在仓库中的个数
	 * @param ItemDef 物品相关定义
	 *
	 * @return 仓库中的数量
	 */
	int32 GetTotalItemCountByDefinition(TSubclassOf<UTD_InventoryItemDefinition> ItemDef) const;

	/**
	 * 通过库存物定义消耗实例对象
	 * @param ItemDef 物品相关定义
	 * @param NumToConsume 消耗数量
	 *
	 * @return 
	 */
	bool ConsumeItemsByDefinition(TSubclassOf<UTD_InventoryItemDefinition> ItemDef, int32 NumToConsume);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of UObject interface

private:
	/** 库存物品列表清单 */
	UPROPERTY(Replicated)
	FTD_InventoryList InventoryList;
};