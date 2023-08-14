#include "GameMode/TD_CharacterBase.h"
#include "AbilitySystem/TD_AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/TD_AttributeSet.h"
#include "Movement/TD_CharacterMovementComponent.h"

ATD_CharacterBase::ATD_CharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTD_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	AbilitySystemComponent = CreateDefaultSubobject<UTD_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UTD_AttributeSet>(TEXT("AttributeSet"));
}

inline UTD_AbilitySystemComponent* ATD_CharacterBase::GetTDAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

inline UTD_AttributeSet* ATD_CharacterBase::GetTDAttributeSet() const
{
	return AttributeSet;
}

UAbilitySystemComponent* ATD_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATD_CharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ATD_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilitySystem();
}

void ATD_CharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATD_CharacterBase::BeginDestroy()
{
	Super::BeginDestroy();
}

void ATD_CharacterBase::InitializeAbilitySystem()
{
	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}
