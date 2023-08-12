#pragma once

#include "CoreMinimal.h"
#include "TD_GameplayAbility.h"
#include "GameplayAbility_PlayMontageAndWaitForEvent.generated.h"

UCLASS()
class UGameplayAbility_PlayMontageAndWaitForEvent : public UTD_GameplayAbility
{
	GENERATED_BODY()
public:
	UGameplayAbility_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};