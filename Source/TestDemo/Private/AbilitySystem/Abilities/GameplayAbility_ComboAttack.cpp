#include "AbilitySystem/Abilities/GameplayAbility_ComboAttack.h"

#include "Component/Fight/TD_FightComponent.h"
#include "GameMode/TD_CharacterBase.h"
#include "Log/TD_Log.h"

UGameplayAbility_ComboAttack::UGameplayAbility_ComboAttack()
{
}

void UGameplayAbility_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (ActorInfo)
	{
		if (ATD_CharacterBase* TempCharacter = Cast<ATD_CharacterBase>(ActorInfo->AvatarActor.Get()))
		{
			if (UTD_FightComponent* FightComponent = TempCharacter->GetFightComponent())
			{
				const int32 TempIndex = FightComponent->GetComboExamine().ComboIndex;
				UE_LOG(TD_GameplayAbility, Warning, TEXT("ComboIndex[%s]"), *FString::FromInt(TempIndex));
				if (TempIndex != INDEX_NONE)
				{
					if (PlayMontage(*FString::FromInt(TempIndex)))
					{

						return;
					}
				}
			}
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	return;
}
