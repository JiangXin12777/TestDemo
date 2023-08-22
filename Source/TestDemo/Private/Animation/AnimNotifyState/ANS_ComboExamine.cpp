#include "Animation/AnimNotifyState/ANS_ComboExamine.h"

#include "Component/Fight/TD_FightComponent.h"
#include "GameMode/TD_CharacterBase.h"
#include "Log/TD_Log.h"

UANS_ComboExamine::UANS_ComboExamine()
{
}

void UANS_ComboExamine::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ATD_CharacterBase* TempCharacter = Cast<ATD_CharacterBase>(MeshComp->GetOuter()))
	{
		if (UTD_FightComponent* FC = TempCharacter->GetFightComponent())
		{
			FC->GetComboExamine().bShortPress = false;
			FC->GetComboExamine().UpdateComboIndex();
		}
	}
}

void UANS_ComboExamine::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_ComboExamine::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ATD_CharacterBase* TempCharacter = Cast<ATD_CharacterBase>(MeshComp->GetOuter()))
	{
		if (UTD_FightComponent* FC = TempCharacter->GetFightComponent())
		{
			if (FC->GetComboExamine().bShortPress || FC->GetComboExamine().bLongPress)
			{
				FC->DoAttack(FC->GetComboExamine().ComboClass);
			}
		}
	}
}
