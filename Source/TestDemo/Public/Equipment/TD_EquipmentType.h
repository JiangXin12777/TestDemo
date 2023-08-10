#pragma once

#include "CoreMinimal.h"
#include "TD_EquipmentType.generated.h"

USTRUCT()
struct FTD_EquipmentActorToSpawn
{
	GENERATED_BODY()

	FTD_EquipmentActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};