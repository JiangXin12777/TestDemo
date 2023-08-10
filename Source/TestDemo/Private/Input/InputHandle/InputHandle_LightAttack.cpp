#include "Input/InputHandle/InputHandle_LightAttack.h"

#include "NinjaInputManagerComponent.h"
#include "AbilitySystem/TD_AbilitySystemComponent.h"
#include "GameplayTag/TD_GameplayTags.h"

UInputHandle_LightAttack::UInputHandle_LightAttack()
	: Super()
{
	ActiveTagContainer = FGameplayTagContainer::EmptyContainer;
	ActiveTagContainer.AddTagFast(TD::Attack_Sword_LightAttack);
	
	TriggerEvents.Add(ETriggerEvent::Started);
}

void UInputHandle_LightAttack::HandleStartedEvent_Implementation(UNinjaInputManagerComponent* Manager,
	const FInputActionValue& Value, const UInputAction* InputAction) const
{
	LightAttack(Manager);
}

bool UInputHandle_LightAttack::CanLightAttack(UNinjaInputManagerComponent* Manager) const
{
	return !HasAnyTags(Manager, BlockTagContainer);
}

void UInputHandle_LightAttack::LightAttack(UNinjaInputManagerComponent* Manager) const
{
	if (!IsValid(Manager) || !CanLightAttack(Manager))
		return;
	
	if (UTD_AbilitySystemComponent* TD_ASC = Cast<UTD_AbilitySystemComponent>(Manager->GetAbilitySystemComponent()))
	{
		TD_ASC->TryActivateAbilitiesByTag(ActiveTagContainer);
	}
}
