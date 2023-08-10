#pragma once

#include "CoreMinimal.h"
#include "System/GameplayTagStack.h"
#include "TD_InventoryItemInstance.generated.h"

class UTD_InventoryItemDefinition;

UCLASS(BlueprintType)
class UTD_InventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UTD_InventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// ~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~End of UObject interface

	/** 将指定数量的堆栈添加到标记（如果堆栈计数低于 1，则不执行任何操作） */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	/** 从标记中删除指定数量的堆栈（如果堆栈计数低于 1，则不执行任何操作） */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category= Inventory)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	/** 返回指定标签的堆栈计数（如果标签不存在则返回 0） */
	UFUNCTION(BlueprintCallable, Category=Inventory)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	/** 如果至少有一堆指定标签，则返回 true */
	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool HasStatTag(FGameplayTag Tag) const;

	TSubclassOf<UTD_InventoryItemDefinition> GetItemDef() const { return ItemDef; }

	/** 查找背包中的物品（碎片） */
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const UTD_InventoryItemFragment* FindFragmentByClass(TSubclassOf<UTD_InventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

private:
#if UE_WITH_IRIS
	/** 注册所有复制片段 */
	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
#endif // UE_WITH_IRIS

	friend struct FTD_InventoryList;

	void SetItemDef(TSubclassOf<UTD_InventoryItemDefinition> InDef);

private:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<UTD_InventoryItemDefinition> ItemDef;
};