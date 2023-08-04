#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TD_CharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class UTD_CharacterMovementComponent
	: public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UTD_CharacterMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};