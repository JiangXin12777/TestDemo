#pragma once

#include "CoreMinimal.h"
#include "NinjaInputHandler.h"
#include "InputHandle_LightAttack.generated.h"

/**
 * 
 */
UCLASS(Abstract, meta = (DisplayName = "Sword: LightAttack"))
class UInputHandle_LightAttack : public UNinjaInputHandler
{
	GENERATED_BODY()
public:
	UInputHandle_LightAttack();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|LightAttack")
	FGameplayTagContainer ActiveTagContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|LightAttack")
	FGameplayTagContainer BlockTagContainer;
	
	virtual void HandleStartedEvent_Implementation(UNinjaInputManagerComponent* Manager, const FInputActionValue& Value,
		const UInputAction* InputAction) const override;

	bool CanLightAttack(UNinjaInputManagerComponent* Manager) const;

	void LightAttack(UNinjaInputManagerComponent* Manager) const;
};