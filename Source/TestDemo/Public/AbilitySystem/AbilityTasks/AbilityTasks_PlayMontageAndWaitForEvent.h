#pragma once

#include "CoreMinimal.h"
#include "TD_AbilityTasks.h"
#include "AbilityTasks_PlayMontageAndWaitForEvent.generated.h"

UCLASS()
class UAbilityTasks_PlayMontageAndWaitForEvent : public UTD_AbilityTasks
{
	GENERATED_BODY()
	
public:
	UAbilityTasks_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};