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

void UTD_AbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer,
	TArray<UGameplayAbility*>& ActiveAbilities) const
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// 迭代所有能力规格的列表
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// 迭代此能力规范的所有实例
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(ActiveAbility);
		}
	}
}

bool UTD_AbilitySystemComponent::IsActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags) const
{
	TArray<UGameplayAbility*> ActiveAbilities;
	GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	return ActiveAbilities.Num() > 0;
}

bool UTD_AbilitySystemComponent::IsActiveAbilitiesWithClass(TSubclassOf<UGameplayAbility> InAbilityClass)
{
	return InAbilityClass ? IsActiveAbilitiesWithTags(InAbilityClass.GetDefaultObject()->AbilityTags) : false;
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
	return Super::AreAbilityTagsBlocked(Tags);
}

void UTD_AbilitySystemComponent::AbilityLocalInputPressed(int32 InputId)
{
	Super::AbilityLocalInputPressed(InputId);
}
