#include "Input/TD_InputSetting.h"
#include "Input/TD_InputTag.h"

UTD_InputSetting::UTD_InputSetting(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TrackedEvents = {
		ETriggerEvent::Started,
		ETriggerEvent::Triggered,
		ETriggerEvent::Ongoing,
		ETriggerEvent::Completed,
		ETriggerEvent::Canceled,
	};
    
	BlockCameraTag = TD::Input_Block_Camera;
	BlockMovementTag = TD::Input_Block_Movement;
	BlockRotationTag = TD::Input_Block_Rotation;
    
	GamepadInputModeTag = TD::Input_Mode_Gamepad;
	KeyboardAndMouseInputModeTag = TD::Input_Mode_KeyboardAndMouse;
    
	bMatchHandlersWithContext = true;
	bShowScreenDebugMessages = false;
	DebugMessageDuration = 5.f;
	DebugMessageColor = FColor::Emerald;
}

FName UTD_InputSetting::GetContainerName() const
{
	return Super::GetContainerName();
}

FName UTD_InputSetting::GetCategoryName() const
{
	static const FName OutCategoryName = TEXT("TD");
	return OutCategoryName;
}

FName UTD_InputSetting::GetSectionName() const
{
	return Super::GetSectionName();
}
