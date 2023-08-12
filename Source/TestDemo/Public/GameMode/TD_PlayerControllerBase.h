#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "TD_PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class ATD_PlayerControllerBase
	: public AModularPlayerController
{
	GENERATED_BODY()

public:
	ATD_PlayerControllerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};