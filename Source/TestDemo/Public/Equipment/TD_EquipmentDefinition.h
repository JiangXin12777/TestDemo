#pragma once

#include "CoreMinimal.h"
#include "TD_EquipmentType.h"
#include "TD_EquipmentDefinition.generated.h"

class UTD_EquipmentInstance;

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

	// Gameplay ability sets to grant when this is equipped
	//UPROPERTY(EditDefaultsOnly, Category=Equipment)
	//TArray<TObjectPtr<const UTD_AbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FTD_EquipmentActorToSpawn> ActorsToSpawn;
};