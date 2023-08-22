#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AttackTrackCheck.generated.h"

class ATD_WeaponActor;

UCLASS()
class UANS_AttackTrackCheck : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UANS_AttackTrackCheck();

protected:
	// ~Begin UAnimNotifyState Interface
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;
	// ~End UAnimNotifyState Interface

protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<ATD_WeaponActor>> WeaponActors;
};