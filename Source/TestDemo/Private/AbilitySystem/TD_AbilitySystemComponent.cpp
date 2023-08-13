#include "AbilitySystem/TD_AbilitySystemComponent.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/Abilities/TD_GameplayAbility.h"

UTD_AbilitySystemComponent::UTD_AbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UTD_AbilitySystemComponent* UTD_AbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	return Cast<UTD_AbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}

bool UTD_AbilitySystemComponent::TryBatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately)
{
	bool AbilityActivated = false;
	if (InAbilityHandle.IsValid())
	{
		FScopedServerAbilityRPCBatcher GSAbilityRPCBatcher(this, InAbilityHandle);
		AbilityActivated = TryActivateAbility(InAbilityHandle, true);

		if (EndAbilityImmediately)
		{
			FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(InAbilityHandle);
			if (AbilitySpec)
			{
				UTD_GameplayAbility* GSAbility = Cast<UTD_GameplayAbility>(AbilitySpec->GetPrimaryInstance());
				GSAbility->ExternalEndAbility();
			}
		}

		return AbilityActivated;
	}

	return AbilityActivated;
}

void UTD_AbilitySystemComponent::BlockAbilitiesWithoutTags(const FGameplayTagContainer& Tags)
{
	BlockedAbilityWithoutTags.UpdateTagCount(Tags, 1);
}

void UTD_AbilitySystemComponent::UnBlockAbilitiesWithoutTags(const FGameplayTagContainer& Tags)
{
	BlockedAbilityWithoutTags.UpdateTagCount(Tags, -1);
}

bool UTD_AbilitySystemComponent::ShouldDoServerAbilityRPCBatch() const
{
	return Super::ShouldDoServerAbilityRPCBatch();
}

void UTD_AbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpecification)
{
	Super::OnGiveAbility(AbilitySpecification);
}

void UTD_AbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpecification)
{
	Super::OnRemoveAbility(AbilitySpecification);
}

bool UTD_AbilitySystemComponent::AreAbilityTagsBlocked(const FGameplayTagContainer& Tags) const
{
	return Super::AreAbilityTagsBlocked(Tags) || (BlockedAbilityWithoutTags.GetExplicitGameplayTags().IsValid() && !Tags.HasAny(BlockedAbilityWithoutTags.GetExplicitGameplayTags()));
}

void UTD_AbilitySystemComponent::AbilityLocalInputPressed(int32 InputId)
{
	Super::AbilityLocalInputPressed(InputId);
}
