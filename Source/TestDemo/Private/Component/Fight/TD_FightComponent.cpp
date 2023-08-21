#include "Component/Fight/TD_FightComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/TD_AbilitySystemComponent.h"

UTD_FightComponent::UTD_FightComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

UAbilitySystemComponent* UTD_FightComponent::GetAbilitySystemComponent() const
{
	check(GetOwner());
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
}

void UTD_FightComponent::TryAttack(FGameplayTagContainer AbilityTags)
{
	if (IsActiveAbilitiesWithTags(AbilityTags))
	{
		
	}
	else
	{
		
	}
}

void UTD_FightComponent::TryBlock()
{
}

void UTD_FightComponent::TrySprint()
{
	
}

bool UTD_FightComponent::IsActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags) const
{
	TArray<UTD_GameplayAbility*> ActiveAbilities;
	GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	return ActiveAbilities.Num() > 0;
}

void UTD_FightComponent::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UTD_GameplayAbility*>& ActiveAbilities) const
{
	if (UTD_AbilitySystemComponent* ASC = GetAbilitySystemComponent<UTD_AbilitySystemComponent>())
	{
		ASC->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	}
}

void UTD_FightComponent::DoAttack(FGameplayTagContainer AbilityTags)
{
}
