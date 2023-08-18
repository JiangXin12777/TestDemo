#pragma once

#include "CoreMinimal.h"
#include "TD_AbilitySetHandle.h"
#include "Engine/DataAsset.h"
#include "TD_AbilitySet.generated.h"

/**
 * 角色能力集合
 */
UCLASS(BlueprintType, Const)
class UTD_AbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UTD_AbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 将能力集授予指定的能力系统组件。返回的句柄稍后可以用来拿走任何已授予的内容。 */
	void GiveToAbilitySystem(UTD_AbilitySystemComponent* InASC, FTD_AbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:
	/** 授予此能力集时授予的游戏能力。 */
	UPROPERTY(EditDefaultsOnly, Category = "TD|Abilities")
	TArray<FTD_AbilitySet_GameplayAbility> GrantedGameplayAbilities;
	
	/** 授予此能力集时授予的游戏效果。 */
	UPROPERTY(EditDefaultsOnly, Category = "TD|Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FTD_AbilitySet_GameplayEffect> GrantedGameplayEffects;

	/** 授予此能力集时要授予的属性集。 */
	UPROPERTY(EditDefaultsOnly, Category = "TD|Attribute Set", meta=(TitleProperty=AttributeSet))
	TArray<FTD_AbilitySet_AttributeSet> GrantedAttributes;
};