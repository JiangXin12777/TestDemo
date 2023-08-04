#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"
#include "TD_PlayerStateBase.generated.h"

class UTD_AbilitySystemComponent;

/**
 * 
 */
UCLASS(Config = Game)
class ATD_PlayerStateBase
	: public APlayerState
	, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	/** !技能组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TD|Ability", meta=(AllowPrivateAccess))
	TObjectPtr<UTD_AbilitySystemComponent> AbilitySystemComponent;

public:
	ATD_PlayerStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End IAbilitySystemInterface
};