#pragma once

#include "CoreMinimal.h"
#include "NinjaInputHandler.h"
#include "InputHandle_AttackBlock.generated.h"

/**
 * 
 */
UCLASS(Abstract, meta = (DisplayName = "Sword: AttackBlock"))
class UInputHandle_AttackBlock : public UNinjaInputHandler
{
	GENERATED_BODY()
public:
	UInputHandle_AttackBlock();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|AttackBlock")
	FGameplayTagContainer AttackBlockContainer;
	
	virtual void HandleStartedEvent_Implementation(UNinjaInputManagerComponent* Manager, const FInputActionValue& Value,
		const UInputAction* InputAction) const override;

	void AttackBlock(UNinjaInputManagerComponent* Manager) const;
};