#include "AbilitySystem/Abilities/GameplayAbility_PlayMontageAndWaitForEvent.h"

UGameplayAbility_PlayMontageAndWaitForEvent::UGameplayAbility_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

int32 UGameplayAbility_PlayMontageAndWaitForEvent::GetCompositeSectionsNumber() const
{
	if (MontageToPlay)
	{
		return MontageToPlay->CompositeSections.Num();
	}
	return INDEX_NONE;
}

void UGameplayAbility_PlayMontageAndWaitForEvent::OnCompleted()
{
	K2_OnCompleted();
}

void UGameplayAbility_PlayMontageAndWaitForEvent::OnBlendOut()
{
	K2_OnBlendOut();
}

void UGameplayAbility_PlayMontageAndWaitForEvent::OnInterrupted()
{
	K2_OnInterrupted();
}

void UGameplayAbility_PlayMontageAndWaitForEvent::OnCancelled()
{
	K2_OnCancelled();
}

void UGameplayAbility_PlayMontageAndWaitForEvent::OnDamageGameplayEvent(FGameplayTag InGameplayTag,
	FGameplayEventData Payload)
{
	K2_OnDamageGameplayEvent(InGameplayTag, Payload);
}
