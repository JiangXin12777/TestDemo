#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ComboExamine.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FComboExamine
{
	GENERATED_BODY()

public:
	FComboExamine();
	
	UPROPERTY()
	int32 ComboIndex;

	UPROPERTY()
	bool bLongPress;

	UPROPERTY()
	bool bShortPress;

	UPROPERTY()
	int32 MaxIndex;

	UPROPERTY()
	TSubclassOf<UGameplayAbility> ComboClass;

	void UpdateComboIndex();

	void Press();

	void Released();

	void Reset();
};