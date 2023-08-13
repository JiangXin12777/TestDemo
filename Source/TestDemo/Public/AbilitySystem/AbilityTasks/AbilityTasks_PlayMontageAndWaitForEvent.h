#pragma once

#include "CoreMinimal.h"
#include "TD_AbilityTasks.h"
#include "AbilityTasks_PlayMontageAndWaitForEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMontageWaitSimpleForDamageEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);

class UUAnimMontage;

/**
 * 异步播放 Montage
 */
UCLASS()
class UAbilityTasks_PlayMontageAndWaitForEvent : public UTD_AbilityTasks
{
	GENERATED_BODY()
	
public:
	UAbilityTasks_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 
	 * Start playing an animation montage on the avatar actor and wait for it to finish
	 * If StopWhenAbilityEnds is true, this montage will be aborted if the ability ends normally. It is always stopped when the ability is explicitly cancelled.
	 * On normal execution, OnBlendOut is called when the montage is blending out, and OnCompleted when it is completely done playing
	 * OnInterrupted is called if another montage overwrites this, and OnCancelled is called if the ability or task is cancelled
	 *
	 * @param OwningAbility 
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param MontageToPlay The montage to play on the character
	 * @param InEventTags 
	 * @param Rate Change to play the montage faster or slower
	 * @param StartSection If not empty, named montage section to start from
	 * @param bStopWhenAbilityEnds If true, this montage will be aborted if the ability ends normally. It is always stopped when the ability is explicitly cancelled
	 * @param AnimRootMotionTranslationScale Change to modify size of root motion or set to 0 to block it entirely
	 * @param StartTimeSeconds Starting time offset in montage, this will be overridden by StartSection if that is also set
	 */
	UFUNCTION(BlueprintCallable, Category="TD|Ability|Tasks", meta = (DisplayName="TD_PlayMontageAndWait", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTasks_PlayMontageAndWaitForEvent* CreatePlayMontageAndWaitProxy(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer InEventTags, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f);

	// ~Begin UGameplayTask Interface
	virtual void Activate() override;
	virtual void ExternalCancel() override;
	virtual FString GetDebugString() const override;
	// ~End UGameplayTask Interface

public:
	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleForDamageEventDelegate	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleForDamageEventDelegate	OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleForDamageEventDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleForDamageEventDelegate	OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleForDamageEventDelegate	DamageEventReceived;

protected:
	// ~Begin UObject Interface
	virtual void OnDestroy(bool AbilityEnded) override;
	// ~End UObject Interface

	/** Checks if the ability is playing a montage and stops that montage, returns true if a montage was stopped, false if not. */
	bool StopPlayingMontage();

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;

	/** 蒙太奇中断或取消的代理句柄 */
	FDelegateHandle CancelHandle;

	/** 执行任务时播放的蒙太奇资产 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Ability|Tasks")
	TObjectPtr<UAnimMontage> MontageToPlay;

	/** 播放速率 */
	UPROPERTY()
	float Rate;

	/** 起始位置的插槽名称 */
	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	float StartTimeSeconds;

	UPROPERTY()
	bool bStopWhenAbilityEnds;

private:
	/** 出发伤害事件时调用 */
	void OnDamageGameplayEvent(FGameplayTag InGameplayTag, const FGameplayEventData* Payload);

	/** 当蒙太奇混出时调用 */
	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	/** 当蒙太奇取消时调用 */
	UFUNCTION()
	void OnMontageCancel();

	/** 蒙太奇结束时调用 */
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY()
	FGameplayTagContainer EventTags;

	FDelegateHandle DamageEventHandle;
};