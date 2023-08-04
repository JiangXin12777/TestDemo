// Ninja Bear Studio Inc. 2023, all rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "NinjaInputHandler.h"
#include "UObject/Object.h"
#include "InputHandler_AbilityActivation.generated.h"

class UGameplayAbility;

/**
 * Base input handler for ability activations. 
 */
UCLASS(Abstract)
class NINJAINPUT_API UInputHandler_AbilityActivation : public UNinjaInputHandler
{
    
    GENERATED_BODY()

public:
    
    UInputHandler_AbilityActivation();
    
protected:

    /**
     * Determines if the activation should be toggled.
     *
     * This means the first successful trigger will activate the ability and the next one will check if the
     * ability is active first (with an optional query test), and if so, it will interrupt the ability.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
    bool bToggledActivation;

    /**
     * If set to true, this handler will send a Gameplay Event on additional inputs, made while the ability is active.
     *
     * Activation must be determined by child classes that will have their own activation strategies (i.e. Class, Tags
     * or Input ID) and must use these same strategies to determine if an ability is active or not. 
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
    bool bSendEventIfActive;

    /**
     * If set to true, the event will be triggered on locally.
     *
     * If the "Trigger Event On Server" flag is enabled and the Input Owner is both locally controlled and authoritative,
     * the event is guaranteed to not be triggered twice.
     */    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation", meta = (EditCondition = "bSendEventIfActive", EditConditionHides))
    bool bTriggerEventLocally;

    /**
     * If set to true, the event will be triggered on the server (authoritative) version.
     *
     * If the "Trigger Event Locally" flag is enabled and the Input Owner is both locally controlled and authoritative,
     * the event is guaranteed to not be triggered twice.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation", meta = (EditCondition = "bSendEventIfActive", EditConditionHides))
    bool bTriggerEventOnServer;
    
    /**
     * Gameplay Tag used to trigger a Gameplay Event, if there is an activation attempt, while the ability is active.
     *
     * This Gameplay Event can be tracked by the active ability or any other ability that is already active or can
     * be activated by a Gameplay State. A common use-case for this is a Combo Attack.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation", meta = (EditCondition = "bSendEventIfActive", EditConditionHides))
    FGameplayTag ActiveEventTag;

    // -- Begin Input Handler implementation
    virtual void HandleTriggeredEvent_Implementation(UNinjaInputManagerComponent* Manager,
        const FInputActionValue& Value, const UInputAction* InputAction) const override;

    virtual void HandleCompletedEvent_Implementation(UNinjaInputManagerComponent* Manager,
        const FInputActionValue& Value, const UInputAction* InputAction) const override;

    virtual void HandleCancelledEvent_Implementation(UNinjaInputManagerComponent* Manager,
        const FInputActionValue& Value, const UInputAction* InputAction) const override;
    // -- End Input Handler implementation

    /**
     * Allows proper handling of abilities that are already handled, based on the current setup.
     *
     * A reason for you to override this method is to potentially avoid iterating over the abilities
     * if there's not reason to do so. For example, by default, both "Event" and Toggle "checks are
     * taken into consideration. If that scenario can never happen in conjunction for you system,
     * then it may be worth it to adjust this functionality to fit your needs.
     *
     * @param Manager           Input Manager that has invoked this handler. Must be valid.
     * @param Value             Original value received from the Enhanced Input System.
     * @param InputAction       Original Action that is triggering this activation.
     * @return                  True if this method handled the ability. If false, then handling is pending.
     */
    virtual bool TryHandleActiveAbility(UNinjaInputManagerComponent* Manager, const FInputActionValue& Value,
        const UInputAction* InputAction) const;

    /**
     * Concrete implementation that can check if an ability is active based on a certain criteria.
     *
     * Each subclass must implement this method properly, without calling the base implementation (super).
     * 
     * @param Manager           Input Manager that has invoked this handler. Must be valid.
     * @return                  True if an Ability is active.
     */
    virtual bool HasActiveAbility(UNinjaInputManagerComponent* Manager) const;

    /**
     * Concrete implementation that will activate an ability using a proper activation mode.
     *
     * Each subclass must implement this method properly, without calling the base implementation (super).
     * 
     * @param Manager           Input Manager that has invoked this handler. Must be valid.
     * @param Value             Original value received from the Enhanced Input System.
     * @param InputAction       Original Action that is triggering this activation.
     */
    virtual void ActivateAbility(UNinjaInputManagerComponent* Manager, const FInputActionValue& Value,
        const UInputAction* InputAction) const;

    /**
     * Concrete implementation that will cancel an ability using a proper cancellation mode.
     * 
     * @param Manager           Input Manager that has invoked this handler. Must be valid.
     * @param Value             Original value received from the Enhanced Input System.
     * @param InputAction       Original Action that is triggering this activation.
     */
    virtual void CancelAbility(UNinjaInputManagerComponent* Manager, const FInputActionValue& Value,
        const UInputAction* InputAction) const;
    
    /**
     * Sends a gameplay event for the provided tag.
     *
     * It will send the Input Action as an optional object and the value as Magnitude.
     *
     * @param Manager           Input Manager that has invoked this handler. Must be valid.
     * @param GameplayEventTag  Gameplay Event Tag that identifies the Event to trigger. Must be valid.
     * @param Value             Input value that generated the input activation.
     * @param InputAction       Specific Input Action that was activated.
     * @return                  Ability activations generated by this event. 
     */
    UFUNCTION(BlueprintCallable, Category = "Ability Activation Input Handler")
    virtual int32 SendGameplayEvent(const UNinjaInputManagerComponent* Manager, FGameplayTag GameplayEventTag,
        const FInputActionValue& Value, const UInputAction* InputAction) const;
    
};
