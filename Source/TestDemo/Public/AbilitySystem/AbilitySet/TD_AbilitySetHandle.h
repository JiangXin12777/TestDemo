#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "TD_AbilitySetHandle.generated.h"

class UTD_GameplayAbility;
class UAttributeSet;
class UGameplayEffect;
class UTD_AbilitySystemComponent;

/**
 * 能力集用于授予游戏能力的数据。
 */
USTRUCT(BlueprintType)
struct FTD_AbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	/** 游戏能力 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTD_GameplayAbility> Ability = nullptr;

	/** 能力级别 */
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	/** 处理能力输入的标签。 */
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * 能力集用于授予游戏效果的数据。
 */
USTRUCT(BlueprintType)
struct FTD_AbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:

	/** 游戏效果授予。 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	/** 游戏效果级别。 */
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

/**
 * 能力集用于授予属性集的数据。
 */
USTRUCT(BlueprintType)
struct FTD_AbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;
};

/**
 * 用于存储已由能力集授予的内容的句柄的数据。
 */
USTRUCT(BlueprintType)
struct FTD_AbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	/** 解除相关能力注册 */
	void TakeFromAbilitySystem(UTD_AbilitySystemComponent* InASC);

protected:
	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	
	UPROPERTY(Transient)
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};
