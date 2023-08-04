#include "Input/Data/TD_InputSetupDataAsset.h"

#include "Input/TD_InputHandle.h"

UTD_InputSetupDataAsset::UTD_InputSetupDataAsset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FPrimaryAssetId UTD_InputSetupDataAsset::GetPrimaryAssetId() const
{
	const FPrimaryAssetType BaseAssetType = TEXT("TD_InputSetup");
	return FPrimaryAssetId(BaseAssetType, GetFName());
}

bool UTD_InputSetupDataAsset::HasCompatibleInputHandler(const UInputAction* InputAction,
	const ETriggerEvent& TriggerEvent) const
{
	return InputHandlers.ContainsByPredicate([InputAction, TriggerEvent](const UTD_InputHandle* Handler)
	   { return IsValid(Handler) && Handler->CanHandle(TriggerEvent, InputAction); });
}
