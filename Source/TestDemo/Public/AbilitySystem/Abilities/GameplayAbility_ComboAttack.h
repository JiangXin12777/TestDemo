#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility_PlayMontageAndWaitForEvent.h"
#include "GameplayAbility_ComboAttack.generated.h"

UCLASS()
class UGameplayAbility_ComboAttack : public UGameplayAbility_PlayMontageAndWaitForEvent
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_ComboAttack();
	
protected:
	// ~Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// ~End UGameplayAbility Interface
};