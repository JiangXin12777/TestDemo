#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TD_FightComponent.generated.h"

class UAbilitySystemComponent;

/**
 * 战斗组件
 */
UCLASS()
class UTD_FightComponent
	: public UActorComponent
{
	GENERATED_BODY()
	
public:
	UTD_FightComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 获取当前的 ASC */
	UFUNCTION(BlueprintCallable, Category = "TD|Abilities")
	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	template <class T>
	FORCEINLINE T* GetAbilitySystemComponent() const
	{
		return CastChecked<T>(GetAbilitySystemComponent());
	}

	UFUNCTION(BlueprintCallable, Category = "TD|Fight")
	void TryAttack(FGameplayTagContainer AbilityTags);

	UFUNCTION(BlueprintCallable, Category = "TD|Fight")
	void TryBlock();
	
	UFUNCTION(BlueprintCallable, Category = "TD|Fight")
	void TrySprint();

protected:
	/**
	 * 当前传入的标签是否为激活状态
	 * 
	 * @param AbilityTags 需要匹配的标签
	 */
	UFUNCTION(BlueprintCallable, Category = "TD|Abilities")
	bool IsActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags) const;

	/**
	 * 通过指定标签获取当前激活的 GA
	 * 
	 * @param AbilityTags 需要匹配的标签
	 * @param ActiveAbilities 返回当前激活的 GA
	 */
	UFUNCTION(BlueprintCallable, Category = "TD|Abilities")
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UTD_GameplayAbility*>& ActiveAbilities) const;

	void DoAttack(FGameplayTagContainer AbilityTags);
};