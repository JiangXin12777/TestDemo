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

	/** 激活当前使用道具的前一个道具 */
	UFUNCTION(BlueprintCallable, Category="TD")
	void CycleActiveSlotForward();

	/** 激活当前使用道具的后一个道具 */
	UFUNCTION(BlueprintCallable, Category="TD")
	void CycleActiveSlotBackward();

	/** 激活对应下标道具 */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="TD")
	void SetActiveSlotIndex(int32 NewIndex);

	/** 获取当前道具栏中所有的道具 */
	UFUNCTION(BlueprintCallable, Category="TD")
	const TArray<UTD_InventoryItemInstance*>& GetSlots() const;

	/** 获取当前激活道具的下标 */
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

	/** 获取当前激活的道具 */
	UFUNCTION(BlueprintCallable, Category="TD")
	UTD_InventoryItemInstance* GetActiveSlotItem() const;

	/** 获取无效的道具插槽 */
	UFUNCTION(BlueprintCallable, Category="TD")
	int32 GetNextFreeItemSlot() const;

	/** 添加道具到指定插槽 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TD")
	void AddItemToSlot(int32 SlotIndex, UTD_InventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="TD")
	UTD_InventoryItemInstance* RemoveItemFromSlot(int32 SlotIndex);

	virtual void BeginPlay() override;

private:
	/** 卸载装备 */
	void UnequipItemInSlot();

	/** 装在装备到插槽 */
	void EquipItemInSlot();

	/** 查找装备管理器 */
	UTD_EquipmentManagerComponent* FindEquipmentManager() const;

protected:
	UPROPERTY()
	int32 NumSlots = 3;

	UFUNCTION()
	void OnRep_Slots();

	UFUNCTION()
	void OnRep_ActiveSlotIndex();

private:
	UPROPERTY(ReplicatedUsing=OnRep_Slots)
	TArray<TObjectPtr<UTD_InventoryItemInstance>> Slots;

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