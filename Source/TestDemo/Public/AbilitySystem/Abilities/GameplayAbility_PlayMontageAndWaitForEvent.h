#pragma once

#include "CoreMinimal.h"
#include "TD_GameplayAbility.h"
#include "GameplayAbility_PlayMontageAndWaitForEvent.generated.h"

class UAnimMontage;

UCLASS()
class UGameplayAbility_PlayMontageAndWaitForEvent : public UTD_GameplayAbility
{
	GENERATED_BODY()
public:
	UGameplayAbility_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 获取当前蒙太奇中动画序列的数量 */
	UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	int32 GetCompositeSectionsNumber() const;

	UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	UAbilityTasks_PlayMontageAndWaitForEvent* PlayMontage(FName InStartSection = NAME_None, float InRate = 1.f, FName InTaskInstanceName = NAME_None, bool bInStopWhenAbilityEnds = true, float InAnimRootMotionTranslationScale = 1.f);

protected:
	/** 完成 */
	UFUNCTION()
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION(BlueprintImplementableEvent, Category = "TD|Ability", DisplayName = "OnCompleted", meta = (ScriptName = "OnCompleted"))
	void K2_OnCompleted(const FGameplayTag& EventTag, const FGameplayEventData& EventData);

	/** 蒙太奇 播放 完成 */
	UFUNCTION()
	virtual void OnBlendOut(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION(BlueprintImplementableEvent, Category = "TD|Ability", DisplayName = "OnBlendOut", meta = (ScriptName = "OnBlendOut"))
	void K2_OnBlendOut(const FGameplayTag& EventTag, const FGameplayEventData& EventData);

	/** 蒙太奇 播放 被中断 */
	UFUNCTION()
	virtual void OnInterrupted(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION(BlueprintImplementableEvent, Category = "TD|Ability", DisplayName = "OnInterrupted", meta = (ScriptName = "OnInterrupted"))
	void K2_OnInterrupted(const FGameplayTag& EventTag, const FGameplayEventData& EventData);

	/** 蒙太奇 播放 被取消 */
	UFUNCTION()
	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION(BlueprintImplementableEvent, Category = "TD|Ability", DisplayName = "OnCancelled", meta = (ScriptName = "OnCancelled"))
	void K2_OnCancelled(const FGameplayTag& EventTag, const FGameplayEventData& EventData);

	/** 命中到敌人时回调 */
	UFUNCTION()
	virtual	void OnDamageGameplayEvent(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION(BlueprintImplementableEvent, Category = "TD|MontageAbility", DisplayName = "OnDamageGameplayEvent", meta = (ScriptName = "OnDamageGameplayEvent"))
	void K2_OnDamageGameplayEvent(const FGameplayTag& EventTag, const FGameplayEventData& EventData);

	UFUNCTION(BlueprintCallable, Category="TD|Ability")
	UAbilityTasks_PlayMontageAndWaitForEvent* CreatePlayMontageAndWaitForEvent(FName InTaskInstanceName = NAME_None, float InRate = 1.f, FName InStartSection = NAME_None, bool bInStopWhenAbilityEnds = true, float InAnimRootMotionTranslationScale = 1.f);
	
protected:
	/** 当前技能所匹配的蒙太奇动画 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|MontageAbility")
	UAnimMontage* MontageToPlay;
};