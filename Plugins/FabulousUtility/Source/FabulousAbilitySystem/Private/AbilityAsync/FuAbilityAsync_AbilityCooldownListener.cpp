#include "AbilityAsync/FuAbilityAsync_AbilityCooldownListener.h"

#include "FuAbilitySystemComponent.h"
#include "FuMacros.h"
#include "Utility/FuEffectUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_AbilityCooldownListener)

UFuAbilityAsync_AbilityCooldownListener* UFuAbilityAsync_AbilityCooldownListener::FuListenForAbilityCooldownByAbilityTagOnActor(
	const AActor* Actor, const FGameplayTag InAbilityTag, const bool bInWaitForTimeFromServer)
{
	return FuListenForAbilityCooldownByAbilityTag(UFuAbilitySystemComponent::GetFuAbilitySystem(Actor),
	                                              InAbilityTag, bInWaitForTimeFromServer);
}

UFuAbilityAsync_AbilityCooldownListener* UFuAbilityAsync_AbilityCooldownListener::FuListenForAbilityCooldownByAbilityTagsOnActor(
	const AActor* Actor, const FGameplayTagContainer InAbilityTags, const bool bInWaitForTimeFromServer)
{
	return FuListenForAbilityCooldownByAbilityTags(UFuAbilitySystemComponent::GetFuAbilitySystem(Actor),
	                                               InAbilityTags, bInWaitForTimeFromServer);
}

UFuAbilityAsync_AbilityCooldownListener* UFuAbilityAsync_AbilityCooldownListener::FuListenForAbilityCooldownByInputIdOnActor(
	const AActor* Actor, const int32 InInputId, const bool bInWaitForTimeFromServer)
{
	return FuListenForAbilityCooldownByInputId(UFuAbilitySystemComponent::GetFuAbilitySystem(Actor),
	                                           InInputId, bInWaitForTimeFromServer);
}

UFuAbilityAsync_AbilityCooldownListener* UFuAbilityAsync_AbilityCooldownListener::FuListenForAbilityCooldownByAbilityTag(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTag InAbilityTag, const bool bInWaitForTimeFromServer)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);

	if (FU_ENSURE(InAbilityTag.IsValid()))
	{
		Task->AbilityTags.AddTag(InAbilityTag);
	}

	Task->bWaitForTimeFromServer = bInWaitForTimeFromServer;

	return Task;
}

UFuAbilityAsync_AbilityCooldownListener* UFuAbilityAsync_AbilityCooldownListener::FuListenForAbilityCooldownByAbilityTags(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer InAbilityTags, const bool bInWaitForTimeFromServer)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);

	for (const auto& Tag : InAbilityTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->AbilityTags.AddTag(Tag);
		}
	}

	Task->bWaitForTimeFromServer = bInWaitForTimeFromServer;

	return Task;
}

UFuAbilityAsync_AbilityCooldownListener* UFuAbilityAsync_AbilityCooldownListener::FuListenForAbilityCooldownByInputId(
	UFuAbilitySystemComponent* AbilitySystem, const int32 InInputId, const bool bInWaitForTimeFromServer)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);
	Task->InputId = InInputId;
	Task->bWaitForTimeFromServer = bInWaitForTimeFromServer;

	return Task;
}

void UFuAbilityAsync_AbilityCooldownListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};

	if (!IsValid(GetAbilitySystemComponent()) || !FU_ENSURE(IsValid(AbilitySystem)) ||
	    (AbilityTags.IsEmpty() && !FU_ENSURE(InputId >= 0)))
	{
		EndAction();
		return;
	}

	AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectAdded);
	AbilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectRemoved);

	AbilitySystem->OnAbilityGiven.AddUObject(this, &ThisClass::AbilitySystem_OnAbilityGiven);
	AbilitySystem->OnAbilityRemoved.AddUObject(this, &ThisClass::AbilitySystem_OnAbilityRemoved);

	for (const auto& AbilitySpecification : AbilitySystem->GetActivatableAbilities())
	{
		if ((InputId >= 0 && AbilitySpecification.InputID == InputId) ||
		    AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags) ||
		    AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags))
		{
			const auto* CooldownTags{AbilitySpecification.Ability->GetCooldownTags()};
			if (CooldownTags != nullptr)
			{
				EffectTags.UpdateTagCount(*CooldownTags, 1);
			}
		}
	}

	for (const auto& EffectTag : EffectTags.GetExplicitGameplayTags())
	{
		AbilitySystem->RegisterGameplayTagEvent(EffectTag, EGameplayTagEventType::NewOrRemoved)
		             .AddUObject(this, &ThisClass::AbilitySystem_OnTagChanged);
	}

	for (auto& ActiveEffect : const_cast<FActiveGameplayEffectsContainer*>(&AbilitySystem->GetActiveGameplayEffects()))
	{
		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasAny(EffectTags.GetExplicitGameplayTags()) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasAny(EffectTags.GetExplicitGameplayTags()))
		{
			ActiveEffect.EventSet.OnTimeChanged.AddUObject(this, &ThisClass::ActiveEffect_OnTimeChanged);
		}
	}

	for (const auto& EffectTag : EffectTags.GetExplicitGameplayTags())
	{
		RefreshEffectTimeRemainingAndDurationForTag(EffectTag);
	}
}

void UFuAbilityAsync_AbilityCooldownListener::EndAction()
{
	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		AbilitySystem->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);

		AbilitySystem->OnAbilityGiven.RemoveAll(this);
		AbilitySystem->OnAbilityRemoved.RemoveAll(this);

		for (const auto& EffectTag : EffectTags.GetExplicitGameplayTags())
		{
			AbilitySystem->RegisterGameplayTagEvent(EffectTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}

		for (auto& ActiveEffect : const_cast<FActiveGameplayEffectsContainer*>(&AbilitySystem->GetActiveGameplayEffects()))
		{
			ActiveEffect.EventSet.OnTimeChanged.RemoveAll(this);
		}
	}

	Super::EndAction();
}

void UFuAbilityAsync_AbilityCooldownListener::ProcessAbilitySpecificationChange(const FGameplayAbilitySpec& AbilitySpecification,
                                                                                const bool bAddedOrRemoved)
{
	if ((InputId < 0 || AbilitySpecification.InputID != InputId) &&
	    !AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags) &&
	    !AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags))
	{
		return;
	}

	const auto* CooldownTags{AbilitySpecification.Ability->GetCooldownTags()};

	if (CooldownTags == nullptr || CooldownTags->IsEmpty())
	{
		return;
	}

	EffectTags.UpdateTagCount(*CooldownTags, bAddedOrRemoved ? 1 : -1);

	auto* AbilitySystem{GetAbilitySystemComponent()};

	for (const auto& CooldownTag : *CooldownTags)
	{
		const auto TagCount{EffectTags.GetTagCount(CooldownTag)};

		if (bAddedOrRemoved && TagCount == 1)
		{
			// A cooldown tag has been added.

			AbilitySystem->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
			             .AddUObject(this, &ThisClass::AbilitySystem_OnTagChanged);

			RefreshEffectTimeRemainingAndDurationForTag(CooldownTag);
		}
		else if (!bAddedOrRemoved && TagCount <= 0)
		{
			// A cooldown tag has been removed.

			AbilitySystem->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
			             .AddUObject(this, &ThisClass::AbilitySystem_OnTagChanged);

			if (ShouldBroadcastDelegates())
			{
				OnEffectEnded.Broadcast(CooldownTag, 0.0f, 0.0f, false);
			}
		}
	}

	// Re-register effect time change events.

	for (auto& ActiveEffect : const_cast<FActiveGameplayEffectsContainer*>(&AbilitySystem->GetActiveGameplayEffects()))
	{
		ActiveEffect.EventSet.OnTimeChanged.RemoveAll(this);

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasAny(EffectTags.GetExplicitGameplayTags()) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasAny(EffectTags.GetExplicitGameplayTags()))
		{
			ActiveEffect.EventSet.OnTimeChanged.AddUObject(this, &ThisClass::ActiveEffect_OnTimeChanged);
		}
	}
}

void UFuAbilityAsync_AbilityCooldownListener::RefreshEffectTimeRemainingAndDurationForTag(const FGameplayTag& EffectTag) const
{
	if (!ShouldBroadcastDelegates())
	{
		return;
	}

	const auto* AbilitySystem{GetAbilitySystemComponent()};

	float TimeRemaining, Duration;

	const auto* ActiveEffect{
		UFuEffectUtility::GetActiveEffectTimeRemainingAndDurationByTag(AbilitySystem, EffectTag, TimeRemaining, Duration)
	};

	if (ActiveEffect == nullptr)
	{
		if (!AbilitySystem->HasMatchingGameplayTag(EffectTag))
		{
			OnEffectEnded.Broadcast(EffectTag, 0.0f, 0.0f, false);
		}

		return;
	}

	if (AbilitySystem->GetOwnerRole() >= ROLE_Authority)
	{
		OnEffectStated.Broadcast(EffectTag, TimeRemaining, Duration, false);
	}
	else if (IsValid(ActiveEffect->Spec.GetContext().GetAbilityInstance_NotReplicated()))
	{
		if (bWaitForTimeFromServer)
		{
			// Waiting for time from the server.
			OnEffectStated.Broadcast(EffectTag, TimeRemaining, Duration, true);
		}
		else
		{
			// Predicted time.
			OnEffectStated.Broadcast(EffectTag, TimeRemaining, Duration, false);
		}
	}
	else if (bWaitForTimeFromServer)
	{
		// Time from the server.
		OnEffectStated.Broadcast(EffectTag, TimeRemaining, Duration, false);
	}
}

void UFuAbilityAsync_AbilityCooldownListener::AbilitySystem_OnAbilityGiven(const FGameplayAbilitySpec& AbilitySpecification)
{
	ProcessAbilitySpecificationChange(AbilitySpecification, true);
}

void UFuAbilityAsync_AbilityCooldownListener::AbilitySystem_OnAbilityRemoved(const FGameplayAbilitySpec& AbilitySpecification)
{
	ProcessAbilitySpecificationChange(AbilitySpecification, false);
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void UFuAbilityAsync_AbilityCooldownListener::AbilitySystem_OnActiveGameplayEffectAdded(
	UAbilitySystemComponent* AbilitySystem, const FGameplayEffectSpec& EffectSpecification,
	const FActiveGameplayEffectHandle EffectHandle) const
{
	auto bEffectTimeChangeEventRegistered{false};

	for (const auto& EffectTag : EffectTags.GetExplicitGameplayTags())
	{
		if (!EffectSpecification.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(EffectTag) &&
		    !EffectSpecification.DynamicGrantedTags.HasTag(EffectTag))
		{
			continue;
		}

		if (!bEffectTimeChangeEventRegistered)
		{
			auto* ActiveEffect{AbilitySystem->GetActiveGameplayEffect(EffectHandle)};
			if (ActiveEffect != nullptr)
			{
				const_cast<FActiveGameplayEffect*>(ActiveEffect)
					->EventSet.OnTimeChanged.AddUObject(this, &ThisClass::ActiveEffect_OnTimeChanged);

				bEffectTimeChangeEventRegistered = true;
			}
		}

		RefreshEffectTimeRemainingAndDurationForTag(EffectTag);
	}
}

void UFuAbilityAsync_AbilityCooldownListener::AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const
{
	const_cast<FActiveGameplayEffect&>(ActiveEffect).EventSet.OnTimeChanged.RemoveAll(this);
}

void UFuAbilityAsync_AbilityCooldownListener::AbilitySystem_OnTagChanged(const FGameplayTag Tag, const int32 Count) const
{
	if (ShouldBroadcastDelegates() && Count <= 0)
	{
		OnEffectEnded.Broadcast(Tag, 0.0f, 0.0f, false);
	}
}

void UFuAbilityAsync_AbilityCooldownListener::ActiveEffect_OnTimeChanged(const FActiveGameplayEffectHandle EffectHandle,
                                                                         const float StartTime, const float Duration) const
{
	auto* ActiveEffect{GetAbilitySystemComponent()->GetActiveGameplayEffect(EffectHandle)};
	if (ActiveEffect == nullptr)
	{
		return;
	}

	for (const auto& EffectTag : EffectTags.GetExplicitGameplayTags())
	{
		if (ActiveEffect->Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(EffectTag) ||
		    ActiveEffect->Spec.DynamicGrantedTags.HasTag(EffectTag))
		{
			RefreshEffectTimeRemainingAndDurationForTag(EffectTag);
		}
	}
}
