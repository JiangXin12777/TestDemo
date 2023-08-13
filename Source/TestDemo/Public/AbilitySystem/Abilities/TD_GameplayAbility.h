#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/Abilities/TD_AbilityType.h"
#include "TD_GameplayAbility.generated.h"

class UTD_AbilitySystemComponent;

UCLASS()
class UTD_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend UTD_AbilitySystemComponent;
	
public:
	UTD_GameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	UTD_AbilitySystemComponent* GetTDAbilitySystemComponentFromActorInfo() const;

protected:
	// ~Begin UGameplayAbility Interface
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual void PreActivate(FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
	// ~End UGameplayAbility Interface

	UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	FGameplayAbilityTargetDataHandle MakeGameplayAbilityTargetDataHandleFromActorArray(const TArray<AActor*> TargetActors);

	UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	FGameplayAbilityTargetDataHandle MakeGameplayAbilityTargetDataHandleFromHitResults(const TArray<FHitResult> HitResults);

	/** 使用传入的容器制作稍后应用的游戏效果容器规范 */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "TD|Event Data"))
	virtual FTD_GameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FTD_GameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** 从 EffectContainerMap 中搜索并制作稍后要应用的游戏效果容器规范 */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "TD|Event Data"))
	virtual FTD_GameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** 应用之前创建的游戏效果容器规范 */
	UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FTD_GameplayEffectContainerSpec& ContainerSpec);

	/** 将 GetSourceObject 公开给蓝图。检索与此能力关联的 SourceObject。可通过非实例化能力调用。 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TD|Ability", meta = (DisplayName = "Get Source Object"))
	UObject* K2_GetSourceObject(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const;

	/** 尝试激活给定的能力句柄并将所有 RPC 批量合并为一个。这只会批处理一帧中发生的所有 RPC。最好的情况是我们将 ActivateAbility、SendTargetData 和 EndAbility 批处理为一个 RPC，而不是三个。 */
	UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	virtual bool TryBatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately);

	/** 与调用 K2_EndAbility 相同。旨在与配料系统一起使用以从外部终止该能力。 */
	virtual void ExternalEndAbility();
	
	/** 返回当前的预测键以及它是否对更多预测有效。用于调试能力预测窗口。 */
	UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	virtual FString GetCurrentPredictionKeyStatus();

	/** 返回当前预测键是否对更多预测有效。 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TD|Ability")
	virtual bool IsPredictionKeyValidForMorePrediction() const;
	
	// Allows C++ and Blueprint abilities to override how cost is checked in case they don't use a GE like weapon ammo
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TD|Ability")
	//bool GSCheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const;
	//virtual bool GSCheckCost_Implementation(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const;

	// Allows C++ and Blueprint abilities to override how cost is applied in case they don't use a GE like weapon ammo
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TD|Ability")
	//void GSApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;
	//virtual void GSApplyCost_Implementation(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const {};

	//UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	//virtual void SetHUDReticle(TSubclassOf<class UGSHUDReticle> ReticleClass);

	//UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	//virtual void ResetHUDReticle();

	// Sends TargetData from the client to the Server and creates a new Prediction Window
	UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	virtual void SendTargetDataToServer(const FGameplayAbilityTargetDataHandle& TargetData);

	// Is the player's input currently pressed? Only works if the ability is bound to input.
	UFUNCTION(BlueprintCallable, Category = "TD|Ability")
	virtual bool IsInputPressed() const;
	
protected:
	/** 游戏标签到游戏效果容器的映射 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Gameplay Effects")
	TMap<FGameplayTag, FTD_GameplayEffectContainer> EffectContainerMap;

	/** 告诉一种能力一旦被授予就立即激活。用于被动能力和强加于他人的能力。 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TD|Ability")
	bool bActivateAbilityOnGranted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Ability")
	FGameplayTagContainer RemoveAbilitiesWithTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Ability")
	FGameplayTagContainer CancelAbilitiesWithoutTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TD|Ability")
	FGameplayTagContainer BlockAbilitiesWithoutTag;
};