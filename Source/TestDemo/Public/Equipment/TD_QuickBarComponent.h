#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "TD_QuickBarComponent.generated.h"

class UTD_InventoryItemInstance;
class UTD_EquipmentInstance;
class UTD_EquipmentManagerComponent;

/**
 * 道具栏组件
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UTD_QuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UTD_QuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// ~UObject interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~End of UObject interface

	// ~Begin Actor Interface
	virtual void BeginPlay() override;
	// ~End Actor Interface

	/** 激活当前使用道具的前一个道具 */
	UFUNCTION(BlueprintCallable, Category="TD|QuickBar")
	void CycleActiveSlotForward();

	/** 激活当前使用道具的后一个道具 */
	UFUNCTION(BlueprintCallable, Category="TD|QuickBar")
	void CycleActiveSlotBackward();

	/**
	 * 激活道具
	 * @param NewIndex 激活的下标
	 */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="TD|QuickBar")
	void SetActiveSlotIndex(int32 NewIndex);

	/**
	 * 激活道具
	 * @param InItem 激活的实例
	 */
	UFUNCTION(BlueprintCallable, Category="TD|QuickBar")
	void SetActiveSlotItem(UTD_InventoryItemInstance* InItem);

	/** 获取当前道具栏中所有的道具 */
	UFUNCTION(BlueprintCallable, Category="TD|QuickBar")
	const TArray<UTD_InventoryItemInstance*>& GetSlots() const;

	/** 获取当前激活道具的下标 */
	UFUNCTION(BlueprintCallable, Category="TD|QuickBar")
	int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

	/** 获取当前激活的道具 */
	UFUNCTION(BlueprintCallable, Category="TD|QuickBar")
	UTD_InventoryItemInstance* GetActiveSlotItem() const;

	/** 获取下一个空闲的道具插槽 */
	UFUNCTION(BlueprintCallable, Category="TD|QuickBar")
	int32 GetNextFreeItemSlot() const;

	/**
	 * 添加道具到指定插槽
	 * @param SlotIndex 插槽位置
	 * @param Item 需要添加的道具实例
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TD|QuickBar")
	void AddItemToSlot(int32 SlotIndex, UTD_InventoryItemInstance* Item);

	/**
	 * 移除指定下标插槽中的道具
	 * @param SlotIndex 插槽下标
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TD|QuickBar")
	UTD_InventoryItemInstance* RemoveItemFromSlot(int32 SlotIndex);

private:
	/** 卸载装备 */
	void UnequipItemInSlot();

	/** 装在装备到插槽 */
	void EquipItemInSlot();

	/** 查找装备管理器 */
	UTD_EquipmentManagerComponent* FindEquipmentManager() const;

protected:
	/** 初始化插槽的数量 */
	UPROPERTY(EditAnywhere, Category = "TD|QuickBar")
	int32 NumSlots = 3;

	UFUNCTION()
	void OnRep_Slots();

	UFUNCTION()
	void OnRep_ActiveSlotIndex();

private:
	/** 当前组件拥有的插槽数量 */
	UPROPERTY(ReplicatedUsing=OnRep_Slots)
	TArray<TObjectPtr<UTD_InventoryItemInstance>> Slots;

	/** 当前激活的道具插槽下标 */
	UPROPERTY(ReplicatedUsing=OnRep_ActiveSlotIndex)
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TObjectPtr<UTD_EquipmentInstance> EquippedItem;
};


USTRUCT(BlueprintType)
struct FTD_QuickBarSlotsChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TArray<TObjectPtr<UTD_InventoryItemInstance>> Slots;
};


USTRUCT(BlueprintType)
struct FTD_QuickBarActiveIndexChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 ActiveIndex = 0;
};