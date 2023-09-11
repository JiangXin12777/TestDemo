#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TD_EquipmentType.h"
#include "TD_EquipmentDefinition.generated.h"

class UTD_EquipmentInstance;
class UTD_AbilitySet;

/**
 * 装备定义
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class UTD_EquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	UTD_EquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UTD_EquipmentInstance> InstanceType;

	/** 武器类型 */
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	FGameplayTag WeaponTypeTag;

	/** 装备此装备后可授予游戏能力 */
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TObjectPtr<const UTD_AbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FTD_EquipmentActorToSpawn> ActorsToSpawn;
};