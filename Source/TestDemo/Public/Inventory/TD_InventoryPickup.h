#pragma once

#include "CoreMinimal.h"
#include "TD_InventoryPickup.generated.h"

class UTD_InventoryItemDefinition;
class UTD_InventoryItemInstance;

USTRUCT(BlueprintType)
struct FTD_PickupTemplate
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 StackCount = 1;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTD_InventoryItemDefinition> ItemDef;
};

USTRUCT(BlueprintType)
struct FTD_PickupInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTD_InventoryItemInstance> Item = nullptr;
};

USTRUCT(BlueprintType)
struct FTD_InventoryPickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTD_PickupInstance> Instances;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTD_PickupTemplate> Templates;
};