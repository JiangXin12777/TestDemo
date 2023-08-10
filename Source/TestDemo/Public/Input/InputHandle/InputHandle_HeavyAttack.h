#pragma once

#include "CoreMinimal.h"
#include "NinjaInputHandler.h"
#include "InputHandle_HeavyAttack.generated.h"

/**
 * 
 */
UCLASS(Abstract, meta = (DisplayName = "Sword: HeavyAttack"))
class UInputHandle_HeavyAttack : public UNinjaInputHandler
{
	GENERATED_BODY()
public:
	UInputHandle_HeavyAttack();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|HeavyAttack")
	FGameplayTagContainer ActiveTagContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|HeavyAttack")
	FGameplayTagContainer BlockTagContainer;
	
	virtual void HandleStartedEvent_Implementation(UNinjaInputManagerComponent* Manager, const FInputActionValue& Value,
		const UInputAction* InputAction) const override;

	bool CanHeavyAttack(UNinjaInputManagerComponent* Manager) const;

	void HeavyAttack(UNinjaInputManagerComponent* Manager) const;
};