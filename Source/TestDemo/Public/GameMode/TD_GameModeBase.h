#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "TD_GameModeBase.generated.h"

/**
 * 
 */
UCLASS(minimalapi)
class ATD_GameModeBase
	: public AGameModeBase
{
	GENERATED_BODY()

public:
	ATD_GameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
