#pragma once

#include "CoreMinimal.h"
#include "NinjaInputHandler.h"
#include "InputHandle_ActivateAbilitiesByTag.generated.h"

/**
 * 通过给定的 GameplayTag 激活 GA，同时可以添加阻止其激活的标签。
 */
UCLASS(Abstract, meta = (DisplayName = "InputHandle: ActivateAbilitiesByTag"))
class UInputHandle_ActivateAbilitiesByTag : public UNinjaInputHandler
{
	GENERATED_BODY()
public:
	UInputHandle_ActivateAbilitiesByTag();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TD|Abilities")
	FGameplayTagContainer ActiveTagContainer;

	// ~Begin UNinjaInputHandler Interface
	virtual void HandleStartedEvent_Implementation(UNinjaInputManagerComponent* Manager, const FInputActionValue& Value, const UInputAction* InputAction) const override;
	// ~End UNinjaInputHandler Interface
	
	void TryActivateAbilitiesByTag(UNinjaInputManagerComponent* Manager) const;
};