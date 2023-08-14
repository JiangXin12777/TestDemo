#include "Input/InputHandle/InputHandle_ActivateAbilitiesByTag.h"

#include "AbilitySystemComponent.h"
#include "NinjaInputManagerComponent.h"

UInputHandle_ActivateAbilitiesByTag::UInputHandle_ActivateAbilitiesByTag()
	: Super()
{
}

void UInputHandle_ActivateAbilitiesByTag::HandleStartedEvent_Implementation(UNinjaInputManagerComponent* Manager,
	const FInputActionValue& Value, const UInputAction* InputAction) const
{
	TryActivateAbilitiesByTag(Manager);
}

bool UInputHandle_ActivateAbilitiesByTag::CanActivateAbilities(UNinjaInputManagerComponent* Manager) const
{
	return !HasAnyTags(Manager, BlockTagContainer);
}

void UInputHandle_ActivateAbilitiesByTag::TryActivateAbilitiesByTag(UNinjaInputManagerComponent* Manager) const
{
	if (!IsValid(Manager) || !CanActivateAbilities(Manager))
		return;
	
	if (UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(Manager->GetAbilitySystemComponent()))
	{
		ASC->TryActivateAbilitiesByTag(ActiveTagContainer);
	}
}
