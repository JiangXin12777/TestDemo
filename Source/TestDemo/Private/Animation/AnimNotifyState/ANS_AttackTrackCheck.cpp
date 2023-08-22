#include "Animation/AnimNotifyState/ANS_AttackTrackCheck.h"

#include "Equipment/TD_QuickBarComponent.h"
#include "GameMode/TD_CharacterBase.h"
#include "Equipment/TD_EquipmentInstance.h"
#include "Weapons/TD_WeaponActor.h"

UANS_AttackTrackCheck::UANS_AttackTrackCheck()
{
}

void UANS_AttackTrackCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp)
	{
		//开启连击窗口
		if (ATD_CharacterBase* OwnerCharacter = Cast<ATD_CharacterBase>(MeshComp->GetOwner()))
		{
			if (UTD_QuickBarComponent* QuickBar = OwnerCharacter->GetQuickBarComponent())
			{
				if (UTD_EquipmentInstance* EquipmentInstance = QuickBar->GetActiveEquippedItem())
				{
					TArray<AActor*> EquipItems = EquipmentInstance->GetSpawnedActors();
					for (auto& Item : EquipItems)
					{
						if (ATD_WeaponActor* TempActor = Cast<ATD_WeaponActor>(Item))
						{
							WeaponActors.Add(TempActor);
							TempActor->ToggleTraceCheck(true);
						}
					}
				}
			}
		}
	}
}

void UANS_AttackTrackCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	for (const auto &Iter : WeaponActors)
	{
		if (Iter != nullptr)
		{
			Iter->ToggleTraceCheck(false);
		}
	}
	WeaponActors.Empty();
}
