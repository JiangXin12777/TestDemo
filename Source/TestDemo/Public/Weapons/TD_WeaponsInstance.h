#pragma once

#include "CoreMinimal.h"
#include "Equipment/TD_EquipmentInstance.h"
#include "TD_WeaponsInstance.generated.h"

/**
 * 武器装备实例
 */
UCLASS()
class UTD_WeaponInstance : public UTD_EquipmentInstance
{
	GENERATED_BODY()
public:
	UTD_WeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ULyraEquipmentInstance interface
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	//~End of ULyraEquipmentInstance interface
};