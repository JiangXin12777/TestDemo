#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TD_HUDBase.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class ATD_HUDBase
	: public AHUD
{
	GENERATED_BODY()

public:
	ATD_HUDBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};