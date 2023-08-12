#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TD_PickupDefinition.generated.h"

class UTD_InventoryItemDefinition;
class UNiagaraSystem;
class USoundBase;

/**
 * 道具拾取相关数据定义
 */
UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "TD Pickup Data", ShortTooltip = "Data asset used to configure a pickup."))
class UTD_PickupDefinition : public UDataAsset
{
	GENERATED_BODY()
public:
	/** 定义要生成的拾取 Actor、授予的能力以及要添加的标签 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Pickup|Equipment")
	TSubclassOf<UTD_InventoryItemDefinition> InventoryItemDefinition;

	/** 展示的道具模型 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Pickup|Mesh")
	TObjectPtr<UStaticMesh> DisplayMesh;

	/** 拾取后再次生成的冷却时间 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Pickup")
	int32 SpawnCoolDownSeconds;

	/** 拾取时播放的音效 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Pickup")
	TObjectPtr<USoundBase> PickedUpSound;

	/** 生成器冷却时间完毕再次生成时播放的音效 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Pickup")
	TObjectPtr<USoundBase> RespawnedSound;

	/** 拾取时播放的粒子特效 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Pickup")
	TObjectPtr<UNiagaraSystem> PickedUpEffect;

	/** 冷却时间完毕后再次生成时播放的粒子特效 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Pickup")
	TObjectPtr<UNiagaraSystem> RespawnedEffect;
};


/**
 * 武器拾取时相关定义
 */
UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "TD Weapon Pickup Data", ShortTooltip = "Data asset used to configure a weapon pickup."))
class  UTD_WeaponPickupDefinition : public UTD_PickupDefinition
{
	GENERATED_BODY()

public:
	/** 武器生成器上展示的 Transform 信息 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Pickup|Mesh")
	FTransform WeaponMeshTransform;
};