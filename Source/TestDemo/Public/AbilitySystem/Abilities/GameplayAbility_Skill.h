#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility_PlayMontageAndWaitForEvent.h"
#include "GameplayAbility_Skill.generated.h"

UCLASS()
class UGameplayAbility_Skill : public UGameplayAbility_PlayMontageAndWaitForEvent
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_Skill();
	
	
};