#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace TD
{
	TESTDEMO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack_LightAttack);
	TESTDEMO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack_HeavyAttack);
	TESTDEMO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack_AttackBlock);

	TESTDEMO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Message_StackChanged);
	
	TESTDEMO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(QuickBar_Message_SlotsChanged);
	TESTDEMO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(QuickBar_Message_ActiveIndexChanged);
}