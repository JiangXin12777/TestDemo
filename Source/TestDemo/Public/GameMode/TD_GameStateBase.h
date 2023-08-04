#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "TD_GameStateBase.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class ATD_GameStateBase
	: public AGameStateBase
{
	GENERATED_BODY()

public:
	ATD_GameStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};