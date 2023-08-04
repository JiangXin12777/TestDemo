#include "Input/TD_InputHandle.h"

#include "AbilitySystemComponent.h"
#include "InputAction.h"
#include "InputTriggers.h"
#include "Input/TD_InputManagerComponent.h"
#include "Input/TD_InputLog.h"
#include "Input/TD_InputSetting.h"

UTD_InputHandle::UTD_InputHandle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCanBeBuffered = false;
}

UWorld* UTD_InputHandle::GetWorld() const
{
	return WorldPtr.IsValid() && WorldPtr->IsValidLowLevelFast() ? WorldPtr.Get() : nullptr;
}

bool UTD_InputHandle::CanHandle_Implementation(const ETriggerEvent& TriggerEvent, const UInputAction* InputAction) const
{
	return TriggerEvent != ETriggerEvent::None && IsValid(InputAction) &&
		InputActions.Contains(InputAction) && TriggerEvents.Contains(TriggerEvent);
}

void UTD_InputHandle::HandleInput(UTD_InputManagerComponent* Manager, const FInputActionValue& Value,
	const ETriggerEvent& TriggerEvent, const UInputAction* InputAction) const
{
	check(IsValid(Manager));

	AddOnScreenDebugMessage(Manager, Value, TriggerEvent, InputAction);
    
	switch (TriggerEvent)
	{
	case ETriggerEvent::Triggered:
		HandleTriggeredEvent(Manager, Value, InputAction);
		break;
	case ETriggerEvent::Started:
		HandleStartedEvent(Manager, Value, InputAction);
		break;
	case ETriggerEvent::Ongoing:
		HandleOngoingEvent(Manager, Value, InputAction);
		break;
	case ETriggerEvent::Canceled:
		HandleCancelledEvent(Manager, Value, InputAction);
		break;
	case ETriggerEvent::Completed:
		HandleCompletedEvent(Manager, Value, InputAction);
		break;
	default:
		const UEnum* EnumPtr = FindObject<UEnum>(GetOuter(), TEXT("ETriggerEvent"), true);
		UE_LOG(TD_Log_InputHandler, Warning, TEXT("[%s] Unable to handle Trigger Event '%s'."),
			*GetNameSafe(Manager->GetOwner()), *EnumPtr->GetValueAsString(TriggerEvent));
	}
}

bool UTD_InputHandle::CanBeBuffered() const
{
	return bCanBeBuffered;
}

void UTD_InputHandle::SetWorld(UWorld* WorldReference)
{
	WorldPtr = WorldReference;
}

void UTD_InputHandle::HandleCompletedEvent_Implementation(UTD_InputManagerComponent* Manager, const FInputActionValue& Value,
                                                          const UInputAction* InputAction) const
{
}

void UTD_InputHandle::HandleCancelledEvent_Implementation(UTD_InputManagerComponent* Manager, const FInputActionValue& Value,
                                                          const UInputAction* InputAction) const
{
}

void UTD_InputHandle::HandleOngoingEvent_Implementation(UTD_InputManagerComponent* Manager, const FInputActionValue& Value,
                                                        const UInputAction* InputAction) const
{
}

void UTD_InputHandle::HandleStartedEvent_Implementation(UTD_InputManagerComponent* Manager, const FInputActionValue& Value,
                                                        const UInputAction* InputAction) const
{
}

void UTD_InputHandle::HandleTriggeredEvent_Implementation(UTD_InputManagerComponent* Manager, const FInputActionValue& Value,
                                                          const UInputAction* InputAction) const
{
}

bool UTD_InputHandle::HasAnyTags(const UTD_InputManagerComponent* Manager, const FGameplayTagContainer& Tags)
{
	check(Manager);

	if (Tags.IsEmpty() == false)
	{
		const UAbilitySystemComponent* AbilityComponent = Manager->GetOwnerAbilitySystemComponent();
		return IsValid(AbilityComponent) && AbilityComponent->HasAnyMatchingGameplayTags(Tags);
	}
	return false;
}

void UTD_InputHandle::AddOnScreenDebugMessage(const UTD_InputManagerComponent* Manager, const FInputActionValue& Value,
	const ETriggerEvent& TriggerEvent, const UInputAction* InputAction) const
{
#if WITH_EDITOR
	if (IsValid(GEngine))
	{
		if (GetDefault<UTD_InputSetting>()->bShowScreenDebugMessages)
		{
			const UEnum* EnumPtr = FindObject<UEnum>(GetOuter(), TEXT("ETriggerEvent"), true);
			const FString DebugMessage = FString::Printf(TEXT("Input Handler Debug: { Class: '%s', Owner: '%s', Action: '%s', Trigger: '%s', Value: '%s' }"),
				*GetNameSafe(this), *GetNameSafe(Manager->GetOwner()), *GetNameSafe(InputAction), *EnumPtr->GetValueAsString(TriggerEvent), *Value.ToString());

			const float Duration = GetDefault<UTD_InputSetting>()->DebugMessageDuration;
			const FColor DebugColor = GetDefault<UTD_InputSetting>()->DebugMessageColor;
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, Duration, DebugColor, DebugMessage);
		}
	}
#endif
}
