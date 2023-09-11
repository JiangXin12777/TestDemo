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
class UTD_WallRunComponent;

UENUM()
enum class ECharacterState : uint8
{
	ECS_None,
	ECS_Idle,
	ECS_Fight,
	ECS_WallRun
};

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

	/** !墙跑组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Character|Wall Run", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTD_WallRunComponent> WallRunComponent;

public:
	ATD_CharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** ！获取 TD 技能组件 */
	UTD_AbilitySystemComponent* GetTDAbilitySystemComponent() const;
	/** ！获取 TD 属性集合 */
	UTD_AttributeSet* GetTDAttributeSet() const;
	/** ！获取 装备管理组件 */
	UTD_EquipmentManagerComponent* GetEquipmentManagerComponent() const;
	/** ！获取 背包组件 */
	UTD_InventoryManagerComponent* GetInventoryManagerComponent() const;
	/** ！获取 装备栏组件 */
	UTD_QuickBarComponent* GetQuickBarComponent() const;
	/** ！获取 武器状态组件 */
	UTD_WeaponStateComponent* GetWeaponStateComponent() const;
	/** ！获取 战斗组件 */
	UTD_FightComponent* GetFightComponent() const;
	/** ！获取墙跑组件 */
	UTD_WallRunComponent* GetWallRunComponent() const;

public:
	/** 调用此函数，只有墙跑激活时才可以调用返回 true */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "TD|Character|Wall Run")
	bool TryWallJump() const;

	/** 停止跳墙动作（相关值恢复正常） */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "TD|Character|Wall Run")
	void StopWallJump() const;

protected:
	// Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End IAbilitySystemInterface

	// ~Begin Actor Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginDestroy() override;
	// ~End  Actor Interface

	// ~Begin APawn Interface
	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	// ~End APawn Interface

	// ~Begin UObject Interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~End UObject Interface

	// ~Begin ReplicatedUsing
	UFUNCTION()
	virtual void OnRep_CurrentCharacterState(ECharacterState OldVal);
	// ~End ReplicatedUsing

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentCharacterState)
	ECharacterState CurrentCharacterState;
};