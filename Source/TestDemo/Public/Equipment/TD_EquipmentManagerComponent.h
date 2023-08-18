#pragma once

#include "CoreMinimal.h"
#include "TD_EquipmentList.h"
#include "Components/PawnComponent.h"
#include "TD_EquipmentManagerComponent.generated.h"

/**
 * Manages equipment applied to a pawn
 */
UCLASS(BlueprintType, Const)
class UTD_EquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UTD_EquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 装载装备 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UTD_EquipmentInstance* EquipItem(TSubclassOf<UTD_EquipmentDefinition> EquipmentDefinition);

	/** 卸载装备 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UTD_EquipmentInstance* ItemInstance);

	// ~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~End of UObject interface

	// ~UActorComponent interface
	// virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	// ~End of UActorComponent interface

	/** 返回给定类型的第一个装备实例，如果没有找到则返回 nullptr */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTD_EquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UTD_EquipmentInstance> InstanceType);
	
	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}
	
	/** 返回给定类型的所有装备实例，如果没有找到，则返回空数组 */
	UFUNCTION(BlueprintPure)
	TArray<UTD_EquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UTD_EquipmentInstance> InstanceType) const;

private:
	UPROPERTY(Replicated)
	FTD_EquipmentList EquipmentList;
};