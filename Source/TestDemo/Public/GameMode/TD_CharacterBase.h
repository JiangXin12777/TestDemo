#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularCharacter.h"
#include "TD_CharacterBase.generated.h"

class UTD_EquipmentManagerComponent;
class UTD_AttributeSet;
class UTD_AbilitySystemComponent;
class UTD_InventoryManagerComponent;
class UTD_QuickBarComponent;
class UTD_WeaponStateComponent;
class UTD_FightComponent;

/**
 * 角色基类
 */
UCLASS(Config = Game)
class ATD_CharacterBase
	: public AModularCharacter
	//: public AAlsCharacter
	//: public ACharacter
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** !技能组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Character|Ability", meta = (AllowPrivateAccess))
	TObjectPtr<UTD_AbilitySystemComponent> AbilitySystemComponent;

	/** !属性集合 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Character|Ability", meta = (AllowPrivateAccess))
	TObjectPtr<UTD_AttributeSet> AttributeSet;

	/** !装备管理组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Character|Equipment", meta = (AllowPrivateAccess))
	TObjectPtr<UTD_EquipmentManagerComponent> EquipmentManagerComponent;

	/** !战斗组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Character|Fight", meta = (AllowPrivateAccess))
	TObjectPtr<UTD_FightComponent> FightComponent;

public:
	ATD_CharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 获取 TD 技能组件 */
	UTD_AbilitySystemComponent* GetTDAbilitySystemComponent() const;
	/** 获取 TD 属性集合 */
	UTD_AttributeSet* GetTDAttributeSet() const;
	/** 获取 装备管理组件 */
	UTD_EquipmentManagerComponent* GetEquipmentManagerComponent() const;
	/** 获取 背包组件 */
	UTD_InventoryManagerComponent* GetInventoryManagerComponent() const;
	/** 获取 装备栏组件 */
	UTD_QuickBarComponent* GetQuickBarComponent() const;
	/** 获取 武器状态组件 */
	UTD_WeaponStateComponent* GetWeaponStateComponent() const;
	/** 获取 战斗组件 */
	UTD_FightComponent* GetFightComponent() const;

protected:
	// Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End IAbilitySystemInterface

	// ~APawn Interface
	virtual void OnRep_PlayerState() override;
	// ~APawn Interface

	// ~Begin Actor Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginDestroy() override;
	// ~End  Actor Interface

	// ~Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	// ~End APawn Interface
};