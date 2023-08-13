#pragma once

// ----------------------------------------------------------------------------------------------------------------
// This header is for Ability-specific structures and enums that are shared across a project
// Every game will probably need a file like this to handle their extensions to the system
// This file is a good place for subclasses of FGameplayEffectContext and FGameplayAbilityTargetData
// ----------------------------------------------------------------------------------------------------------------

#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectRemoved.h"
#include "TD_AbilityType.generated.h"

class UGameplayEffect;
class UTD_TargetType;

USTRUCT(BlueprintType)
struct FTD_GameplayEffectContainer
{
	GENERATED_BODY()

public:
	FTD_GameplayEffectContainer() {}

	/** 设置定位发生的方式 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TSubclassOf<UTD_TargetType> TargetType;

	/** 应用于目标的游戏效果列表 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;
};

USTRUCT(BlueprintType)
struct FTD_GameplayEffectContainerSpec
{
	GENERATED_BODY()

public:
	FTD_GameplayEffectContainerSpec() {}

	/** 目标数据 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	FGameplayAbilityTargetDataHandle TargetData;

	/** 应用于目标的游戏效果实例句柄列表 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

	/** 如果具有任何有效的效果规格，则返回 true */
	bool HasValidEffects() const;

	/** 如果有任何有效目标，则返回 true */
	bool HasValidTargets() const;

	/** 将新目标添加到目标数据 */
	void AddTargets(const TArray<FGameplayAbilityTargetDataHandle>& InTargetData, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);

	/** 清除目标数据 */
	void ClearTargets();
};