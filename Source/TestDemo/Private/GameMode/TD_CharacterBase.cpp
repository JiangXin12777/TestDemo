#include "GameMode/TD_CharacterBase.h"
#include "AbilitySystem/TD_AbilitySystemComponent.h"
#include "GameMode/TD_PlayerStateBase.h"
#include "Movement/TD_CharacterMovementComponent.h"

ATD_CharacterBase::ATD_CharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTD_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
}

UTD_AbilitySystemComponent* ATD_CharacterBase::GetTDAbilitySystemComponent() const
{
	return Cast<UTD_AbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* ATD_CharacterBase::GetAbilitySystemComponent() const
{
	if (const IAbilitySystemInterface* TempInterface = Cast<IAbilitySystemInterface>(GetPlayerState()))
	{
		return TempInterface->GetAbilitySystemComponent();
	}
	return nullptr;
}
