#include "GameplayTag/TD_GameplayTags.h"

namespace TD
{
	UE_DEFINE_GAMEPLAY_TAG(Attack_Sword_LightAttack, "TD.Attack.Sword.LightAttack");
	UE_DEFINE_GAMEPLAY_TAG(Attack_Sword_HeavyAttack, "TD.Attack.Sword.HeavyAttack");
	UE_DEFINE_GAMEPLAY_TAG(Attack_Sword_AttackBlock, "TD.Attack.Sword.AttackBlock");
	
	UE_DEFINE_GAMEPLAY_TAG(Inventory_Message_StackChanged, "TD.Inventory.Message.StackChanged");
	//UE_DEFINE_GAMEPLAY_TAG_STATIC(Inventory_Message_StackChanged, "TD.Inventory.Message.StackChanged");

	UE_DEFINE_GAMEPLAY_TAG(QuickBar_Message_SlotsChanged, "TD.QuickBar.Message.SlotsChanged");
	UE_DEFINE_GAMEPLAY_TAG(QuickBar_Message_ActiveIndexChanged, "TD.QuickBar.Message.ActiveIndexChanged");
}