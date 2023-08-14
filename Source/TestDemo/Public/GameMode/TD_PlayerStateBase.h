#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "TD_PlayerStateBase.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class ATD_PlayerStateBase
	: public AModularPlayerState
{
	GENERATED_BODY()

public:
	ATD_PlayerStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};