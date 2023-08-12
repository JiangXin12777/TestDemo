#pragma once

#include "CoreMinimal.h"
#include "TD_EquipmentType.generated.h"

/**
 * （角色拾取）生成武器时的相关参数
 */
USTRUCT()
struct FTD_EquipmentActorToSpawn
{
	GENERATED_BODY()

	FTD_EquipmentActorToSpawn()
	{}

	/** 生成的 Actor */
	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	/** 附加到插槽名称 */
	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	/** 附加后 Actor 的偏移信息 */
	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};