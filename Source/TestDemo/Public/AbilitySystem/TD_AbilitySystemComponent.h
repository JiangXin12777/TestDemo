#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TD_AbilitySystemComponent.generated.h"

class UTD_GameplayAbility;

/**
 * 技能组件基类
 */
UCLASS()
class UTD_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UTD_AbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static UTD_AbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	/**
	 * @brief	尝试激活给定的能力句柄并将所有 RPC 批量合并为一个。
	 *			这只会批处理一帧中发生的所有 RPC。
	 *			最好的情况是我们将 ActivateAbility、SendTargetData 和 EndAbility 批处理为一个 RPC，而不是三个。
	 *			最坏的情况是，我们将 ActivateAbility 和 SendTargetData 批处理到一个 RPC（而不是两个）中，并稍后在单独的 RPC 中调用 EndAbility。
	 *			如果我们无法使用ActivateAbility对SendTargetData或EndAbility进行批处理，因为它们不会在同一帧中发生，例如由于潜在能力任务，那么批处理没有帮助，我们应该正常激活。
	 *			单次射击（半自动射击）将ActivateAbility、SendTargetData 和EndAbility 合并到一个RPC 中，而不是三个。
	 *			全自动射击将ActivateAbility 和SendTargetData 合并到一个RPC 中，而不是第一个项目符号中的两个。
	 *			随后的每个项目符号都是 SendTargetData 的一个 RPC。当我们完成发射后，我们会为 EndAbility 发送一个最终 RPC。
	 * @param InAbilityHandle 
	 * @param EndAbilityImmediately
	 * 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category = "TD|Abilities")
	virtual bool TryBatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately);

	/**
	 * 返回与标签匹配的当前活动能力实例的列表
	 *
	 * @param GameplayTagContainer 需要匹配那些标签
	 * @param ActiveAbilities 返回当前激活的 GA
	 */
	UFUNCTION(BlueprintCallable, Category = "TD|Abilities")
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UGameplayAbility*>& ActiveAbilities) const;

	/**
	 * 当前传入的标签是否为激活状态
	 * 
	 * @param AbilityTags 需要匹配的标签
	 */
	UFUNCTION(BlueprintCallable, Category = "TD|Abilities")
	bool IsActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags) const;

	/**
	 * 当前传入的类型是否为激活状态
	 *
	 * @param InAbilityClass 需要匹配的类型
	 */
	UFUNCTION(BlueprintCallable, Category = "TD|Abilities")
	bool IsActiveAbilitiesWithClass(TSubclassOf<UGameplayAbility> InAbilityClass);

protected:
	// ~Begin UAbilitySystemComponent Interface
	virtual bool ShouldDoServerAbilityRPCBatch() const override;
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpecification) override;
	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpecification) override;
	virtual bool AreAbilityTagsBlocked(const FGameplayTagContainer& Tags) const override;
	virtual void AbilityLocalInputPressed(int32 InputId) override;
	// ~End UAbilitySystemComponent Interface
};