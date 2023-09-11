#pragma once

#include "CoreMinimal.h"
#include "TD_MovementType.generated.h"

UENUM(BlueprintType)
enum class EWallSide : uint8
{
	EWS_Left,
	EWS_Right
};