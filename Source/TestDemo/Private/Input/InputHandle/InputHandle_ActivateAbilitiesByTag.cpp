#include "Input/InputHandle/InputHandle_ActivateAbilitiesByTag.h"

#include "AbilitySystemComponent.h"
#include "NinjaInputManagerComponent.h"

UInputHandle_ActivateAbilitiesByTag::UInputHandle_ActivateAbilitiesByTag()
	: Super()
{
	TriggerEvents.Add(ETriggerEvent::Started);
}

void UInputHandle_ActivateAbilitiesByTag::HandleStartedEvent_Implementation(UNinjaInputManagerComponent* Manager,
	const FInputActionValue& Value, const UInputAction* InputAction) const
{
	TryActivateAbilitiesByTag(Manager);
}

void UInputHandle_ActivateAbilitiesByTag::TryActivateAbilitiesByTag(UNinjaInputManagerComponent* Manager) const
{
	check(Manager);
	
	if (UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(Manager->GetAbilitySystemComponent()))
	{
		ASC->TryActivateAbilitiesByTag(ActiveTagContainer);
	}
}
