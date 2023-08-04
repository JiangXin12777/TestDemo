#include "GameMode/TD_PlayerStateBase.h"

#include "AbilitySystem/TD_AbilitySystemComponent.h"

ATD_PlayerStateBase::ATD_PlayerStateBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UTD_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

UAbilitySystemComponent* ATD_PlayerStateBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
