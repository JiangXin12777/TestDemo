#include "GameMode/TD_CharacterBase.h"
#include "AbilitySystem/TD_AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/TD_AttributeSet.h"
#include "Component/Fight/TD_FightComponent.h"
#include "Equipment/TD_EquipmentManagerComponent.h"
#include "GameMode/TD_PlayerControllerBase.h"
#include "Movement/TD_CharacterMovementComponent.h"
#include "Movement/TD_WallRunComponent.h"
#include "Net/UnrealNetwork.h"

ATD_CharacterBase::ATD_CharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTD_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	AbilitySystemComponent = CreateDefaultSubobject<UTD_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UTD_AttributeSet>(TEXT("AttributeSet"));

	EquipmentManagerComponent = CreateDefaultSubobject<UTD_EquipmentManagerComponent>(TEXT("EquipmentManagerComponent"));

	FightComponent = CreateDefaultSubobject<UTD_FightComponent>(TEXT("FightComponent"));

	WallRunComponent = CreateDefaultSubobject<UTD_WallRunComponent>(TEXT("WallRunComponent"));
}

UTD_AbilitySystemComponent* ATD_CharacterBase::GetTDAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UTD_AttributeSet* ATD_CharacterBase::GetTDAttributeSet() const
{
	return AttributeSet;
}

UTD_EquipmentManagerComponent* ATD_CharacterBase::GetEquipmentManagerComponent() const
{
	return EquipmentManagerComponent;
}

UTD_InventoryManagerComponent* ATD_CharacterBase::GetInventoryManagerComponent() const
{
	if (ATD_PlayerControllerBase* PC = Cast<ATD_PlayerControllerBase>(GetController()))
	{
		return PC->GetInventoryManagerComponent();
	}
	return nullptr;
}

UTD_QuickBarComponent* ATD_CharacterBase::GetQuickBarComponent() const
{
	if (ATD_PlayerControllerBase* PC = Cast<ATD_PlayerControllerBase>(GetController()))
	{
		return PC->GetQuickBarComponent();
	}
	return nullptr;
}

UTD_WeaponStateComponent* ATD_CharacterBase::GetWeaponStateComponent() const
{
	if (ATD_PlayerControllerBase* PC = Cast<ATD_PlayerControllerBase>(GetController()))
	{
		return PC->GetWeaponStateComponent();
	}
	return nullptr;
}

UTD_FightComponent* ATD_CharacterBase::GetFightComponent() const
{
	return FightComponent;
}

UTD_WallRunComponent* ATD_CharacterBase::GetWallRunComponent() const
{
	return WallRunComponent;
}

bool ATD_CharacterBase::TryWallJump() const
{
	check(WallRunComponent);
	return WallRunComponent->StartWallJump();
}

void ATD_CharacterBase::StopWallJump() const
{
	check(WallRunComponent);
	WallRunComponent->StopWallJump();
}

UAbilitySystemComponent* ATD_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATD_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATD_CharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATD_CharacterBase::BeginDestroy()
{
	Super::BeginDestroy();
}

void ATD_CharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ATD_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void ATD_CharacterBase::UnPossessed()
{
	Super::UnPossessed();
}

void ATD_CharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();

	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void ATD_CharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentCharacterState);
}

void ATD_CharacterBase::OnRep_CurrentCharacterState(ECharacterState OldVal)
{
}
