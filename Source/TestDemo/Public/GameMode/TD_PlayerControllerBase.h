#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "TD_PlayerControllerBase.generated.h"

class UTD_WeaponStateComponent;
class UTD_QuickBarComponent;
class UTD_InventoryManagerComponent;

/**
 * 
 */
UCLASS(Config = Game)
class ATD_PlayerControllerBase
	: public AModularPlayerController
{
	GENERATED_BODY()

	/** ！背包组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Controller|Inventory", meta = (AllowPrivateAccess))
	TObjectPtr<UTD_InventoryManagerComponent> InventoryManagerComponent;

	/** ！装备栏组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Controller|QuickBar", meta = (AllowPrivateAccess))
	TObjectPtr<UTD_QuickBarComponent> QuickBarComponent;

	/** ！武器状态组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Controller|Weapon", meta = (AllowPrivateAccess))
	TObjectPtr<UTD_WeaponStateComponent> WeaponStateComponent;
	
public:
	ATD_PlayerControllerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 获取背包组件 */
	UTD_InventoryManagerComponent* GetInventoryManagerComponent() const;
	/** 获取装备栏组件 */
	UTD_QuickBarComponent* GetQuickBarComponent() const;
	/** 获取武器状态组件 */
	UTD_WeaponStateComponent* GetWeaponStateComponent() const;
};