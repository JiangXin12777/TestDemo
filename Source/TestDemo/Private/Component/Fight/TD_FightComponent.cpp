#include "Component/Fight/TD_FightComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/TD_AbilitySystemComponent.h"
#include "GameplayTag/TD_GameplayTags.h"
#include "Log/TD_Log.h"

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

void UTD_FightComponent::TryComboAttack(TSubclassOf<UGameplayAbility> InAbilityClass)
{
	if (InAbilityClass)
	{
		bool bFirstAttack = false;
	
		if (!IsActiveAbilitiesWithClass(InAbilityClass) || ComboExamine.ComboClass != InAbilityClass)
		{
			ComboExamine.Reset();
			ComboExamine.ComboClass = InAbilityClass;
		
			bFirstAttack = true;
		}
	
		ComboExamine.Press();

		if (bFirstAttack)
		{
			DoAttack(InAbilityClass);
		}
	}
	else
	{
		UE_LOG(TD_GameplayAbility, Warning, TEXT("TryComboAttack:: InAbilityClass is null."));
	}
}

void UTD_FightComponent::DoAttack(TSubclassOf<UGameplayAbility> InAbilityClass) const
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->TryActivateAbilityByClass(InAbilityClass);
	}
}

void UTD_FightComponent::Released()
{
	ComboExamine.Released();
}

void UTD_FightComponent::TryBlock()
{
	if (UTD_AbilitySystemComponent* ASC = GetAbilitySystemComponent<UTD_AbilitySystemComponent>())
	{
		ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TD::Attack_AttackBlock));
	}
}

void UTD_FightComponent::TrySprint()
{
	if (UTD_AbilitySystemComponent* ASC = GetAbilitySystemComponent<UTD_AbilitySystemComponent>())
	{
		ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TD::Attack_Sprint));
	}
}

bool UTD_FightComponent::IsActiveAbilitiesWithClass(TSubclassOf<UGameplayAbility> InAbilityClass) const
{
	if (UTD_AbilitySystemComponent* ASC = GetAbilitySystemComponent<UTD_AbilitySystemComponent>())
	{
		return ASC->IsActiveAbilitiesWithClass(InAbilityClass);
	}
	return false;
}