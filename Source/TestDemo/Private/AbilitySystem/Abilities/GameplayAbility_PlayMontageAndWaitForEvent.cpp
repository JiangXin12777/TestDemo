#include "AbilitySystem/Abilities/GameplayAbility_PlayMontageAndWaitForEvent.h"

#include "AbilitySystem/AbilityTasks/AbilityTasks_PlayMontageAndWaitForEvent.h"

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

UAbilityTasks_PlayMontageAndWaitForEvent* UGameplayAbility_PlayMontageAndWaitForEvent::PlayMontage(FName InStartSection,
	float InRate, FName InTaskInstanceName, bool bInStopWhenAbilityEnds, float InAnimRootMotionTranslationScale)
{
	return CreatePlayMontageAndWaitForEvent(InTaskInstanceName, InRate, InStartSection, bInStopWhenAbilityEnds, InAnimRootMotionTranslationScale);	
}

void UGameplayAbility_PlayMontageAndWaitForEvent::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	K2_OnCompleted(EventTag, EventData);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameplayAbility_PlayMontageAndWaitForEvent::OnBlendOut(FGameplayTag EventTag, FGameplayEventData EventData)
{
	K2_OnBlendOut(EventTag, EventData);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameplayAbility_PlayMontageAndWaitForEvent::OnInterrupted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	K2_OnInterrupted(EventTag, EventData);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameplayAbility_PlayMontageAndWaitForEvent::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	K2_OnCancelled(EventTag, EventData);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameplayAbility_PlayMontageAndWaitForEvent::OnDamageGameplayEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	K2_OnDamageGameplayEvent(EventTag, EventData);
}

UAbilityTasks_PlayMontageAndWaitForEvent* UGameplayAbility_PlayMontageAndWaitForEvent::CreatePlayMontageAndWaitForEvent(
	FName InTaskInstanceName, float InRate, FName InStartSection,
	bool bInStopWhenAbilityEnds, float InAnimRootMotionTranslationScale)
{
	if (UAbilityTasks_PlayMontageAndWaitForEvent* InTask = UAbilityTasks_PlayMontageAndWaitForEvent::CreatePlayMontageAndWaitProxy(
		this,
		InTaskInstanceName,
		MontageToPlay,
		AbilityTags,
		InRate,
		InStartSection,
		bInStopWhenAbilityEnds,
		InAnimRootMotionTranslationScale))
	{
		InTask->OnBlendOut.AddDynamic(this, &ThisClass::OnBlendOut);
		InTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted);
		InTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelled);
		InTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterrupted);
		InTask->OnDamageEventReceived.AddDynamic(this, &ThisClass::OnDamageGameplayEvent);

		InTask->Activate();

		return InTask;
	}

	return nullptr;
}
