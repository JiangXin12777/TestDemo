#include "GameplayTag/TD_GameplayTags.h"

namespace TD
{
	UE_DEFINE_GAMEPLAY_TAG(Attack_LightAttack, "TD.Attack.LightAttack");
	UE_DEFINE_GAMEPLAY_TAG(Attack_HeavyAttack, "TD.Attack.HeavyAttack");
	UE_DEFINE_GAMEPLAY_TAG(Attack_AttackBlock, "TD.Attack.AttackBlock");
	UE_DEFINE_GAMEPLAY_TAG(Attack_Sprint, "TD.Attack.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(Attack_Roll, "TD.Attack.Roll");
	UE_DEFINE_GAMEPLAY_TAG(Attack_Doge, "TD.Attack.Doge");
	
	UE_DEFINE_GAMEPLAY_TAG(Inventory_Message_StackChanged, "TD.Inventory.Message.StackChanged");
	//UE_DEFINE_GAMEPLAY_TAG_STATIC(Inventory_Message_StackChanged, "TD.Inventory.Message.StackChanged");

	UE_DEFINE_GAMEPLAY_TAG(QuickBar_Message_SlotsChanged, "TD.QuickBar.Message.SlotsChanged");
	UE_DEFINE_GAMEPLAY_TAG(QuickBar_Message_ActiveIndexChanged, "TD.QuickBar.Message.ActiveIndexChanged");
}