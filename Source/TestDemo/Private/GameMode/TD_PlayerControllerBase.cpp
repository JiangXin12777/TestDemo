#include "GameMode/TD_PlayerControllerBase.h"

#include "Equipment/TD_QuickBarComponent.h"
#include "Inventory/TD_InventoryManagerComponent.h"
#include "Weapons/TD_WeaponStateComponent.h"

ATD_PlayerControllerBase::ATD_PlayerControllerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InventoryManagerComponent = CreateDefaultSubobject<UTD_InventoryManagerComponent>(TEXT("InventoryManagerComponent"));
	QuickBarComponent = CreateDefaultSubobject<UTD_QuickBarComponent>(TEXT("QuickBarComponent"));
	WeaponStateComponent = CreateDefaultSubobject<UTD_WeaponStateComponent>(TEXT("WeaponStateComponent"));
}

UTD_InventoryManagerComponent* ATD_PlayerControllerBase::GetInventoryManagerComponent() const
{
	return InventoryManagerComponent;
}

UTD_QuickBarComponent* ATD_PlayerControllerBase::GetQuickBarComponent() const
{
	return QuickBarComponent;
}

UTD_WeaponStateComponent* ATD_PlayerControllerBase::GetWeaponStateComponent() const
{
	return WeaponStateComponent;
}
