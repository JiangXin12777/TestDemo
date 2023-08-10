#include "Input/InputHandle/InputHandle_HeavyAttack.h"

#include "NinjaInputManagerComponent.h"
#include "AbilitySystem/TD_AbilitySystemComponent.h"
#include "GameplayTag/TD_GameplayTags.h"

UInputHandle_HeavyAttack::UInputHandle_HeavyAttack()
	: Super()
{
	ActiveTagContainer = FGameplayTagContainer::EmptyContainer;
	ActiveTagContainer.AddTagFast(TD::Attack_Sword_HeavyAttack);
	
	TriggerEvents.Add(ETriggerEvent::Started);
}

void UInputHandle_HeavyAttack::HandleStartedEvent_Implementation(UNinjaInputManagerComponent* Manager,
	const FInputActionValue& Value, const UInputAction* InputAction) const
{
	HeavyAttack(Manager);
}

bool UInputHandle_HeavyAttack::CanHeavyAttack(UNinjaInputManagerComponent* Manager) const
{
	return !HasAnyTags(Manager, BlockTagContainer);
}

void UInputHandle_HeavyAttack::HeavyAttack(UNinjaInputManagerComponent* Manager) const
{
	if (!IsValid(Manager) || !CanHeavyAttack(Manager))
		return;
	
	if (UTD_AbilitySystemComponent* TD_ASC = Cast<UTD_AbilitySystemComponent>(Manager->GetAbilitySystemComponent()))
	{
		TD_ASC->TryActivateAbilitiesByTag(ActiveTagContainer);
	}
}
