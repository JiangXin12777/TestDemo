#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "TD_WeaponStateComponent.generated.h"

/**
 * 武器装备实例
 */
UCLASS()
class UTD_WeaponStateComponent : public UControllerComponent
{
	GENERATED_BODY()
public:
	UTD_WeaponStateComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};