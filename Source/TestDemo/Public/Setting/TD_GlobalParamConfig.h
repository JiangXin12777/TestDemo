#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TD_GlobalParamConfig.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Param Config"))
class UTD_GlobalParamConfig
	: public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UTD_GlobalParamConfig(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;
};