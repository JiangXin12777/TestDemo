#include "Input/InputHandle/InputHandle_AttackBlock.h"

#include "NinjaInputManagerComponent.h"
#include "AbilitySystem/TD_AbilitySystemComponent.h"
#include "GameplayTag/TD_GameplayTags.h"

UInputHandle_AttackBlock::UInputHandle_AttackBlock()
	: Super()
{
	AttackBlockContainer = FGameplayTagContainer::EmptyContainer;
	AttackBlockContainer.AddTagFast(TD::Attack_Sword_AttackBlock);
	
	TriggerEvents.Add(ETriggerEvent::Started);
}

void UInputHandle_AttackBlock::HandleStartedEvent_Implementation(UNinjaInputManagerComponent* Manager,
	const FInputActionValue& Value, const UInputAction* InputAction) const
{
	AttackBlock(Manager);
}

void UInputHandle_AttackBlock::AttackBlock(UNinjaInputManagerComponent* Manager) const
{
	if (!IsValid(Manager))
		return;
	
	if (UTD_AbilitySystemComponent* TD_ASC = Cast<UTD_AbilitySystemComponent>(Manager->GetAbilitySystemComponent()))
	{
		TD_ASC->TryActivateAbilitiesByTag(AttackBlockContainer);
	}
}
