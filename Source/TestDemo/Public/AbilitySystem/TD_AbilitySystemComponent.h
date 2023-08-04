#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TD_AbilitySystemComponent.generated.h"

/**
 * 技能组件基类
 */
UCLASS()
class UTD_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UTD_AbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};