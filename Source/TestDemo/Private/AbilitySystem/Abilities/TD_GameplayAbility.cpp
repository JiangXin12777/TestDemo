#include "AbilitySystem/Abilities/TD_GameplayAbility.h"

#include "AbilitySystem/TD_AbilitySystemComponent.h"

UTD_GameplayAbility::UTD_GameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UTD_AbilitySystemComponent* UTD_GameplayAbility::GetTDAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UTD_AbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}
